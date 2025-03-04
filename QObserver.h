#pragma once

#include "QPorts.h"

#include <cassert>
#include <functional>
#include <list>
#include <type_traits>
#include <variant>

namespace QApp {

namespace Library {

namespace detail {

struct ServiceData {
  enum class Command { Subscribe, Unsubscribe };
  Command cmd;
};

struct Empty {};

template<class TData>
class NotifyCallback {
protected:
  using ServiceData = detail::ServiceData;
  using CData = std::variant<TData, ServiceData>;

private:
  using Arg = TData&&;
  using CSignature = void(Arg);
  using CAction = std::function<CSignature>;

protected:
  template<class T>
  NotifyCallback(T&& onNotify) : onNotify_(std::forward<T>(onNotify)) {
    assert(onNotify_);
  }

  void onNotify(Arg data) const {
    onNotify_(std::move(data));
  }

private:
  CAction onNotify_;
};

template<>
class NotifyCallback<void> {
protected:
  using ServiceData = detail::ServiceData;
  using CData = std::variant<Empty, ServiceData>;

private:
  using CSignature = void();
  using CAction = std::function<CSignature>;

protected:
  template<class T>
  NotifyCallback(T&& onNotify) : onNotify_(std::forward<T>(onNotify)) {
    assert(onNotify_);
  }

  void onNotify(Empty) const {
    onNotify_();
  }

private:
  CAction onNotify_;
};

} // namespace detail

template<class TData>
class CObserver;

template<class TData>
class CObservable;

template<class TData>
class CObserver : protected QPort, protected detail::NotifyCallback<TData> {
  using NotifyCallback = detail::NotifyCallback<TData>;
  using ServiceData = typename NotifyCallback::ServiceData;
  using CData = typename NotifyCallback::CData;
  using CObservable = CObservable<TData>;

public:
  using CPointer = QPort::CPointer;

private:
  using ServiceData = detail::ServiceData;

  friend CObservable;

public:
  template<class T>
  CObserver(T&& onNotify) : NotifyCallback(std::forward<T>(onNotify)) {
  }

  CObserver(const CObserver&) = delete;
  CObserver(CObserver&&) noexcept = delete;
  CObserver& operator=(const CObserver&) = delete;
  CObserver& operator=(CObserver&&) noexcept = delete;
  ~CObserver() {
    unsubscribe();
  }

  void unsubscribe() {
    if (!isSubscribed())
      return;
    sendServiceMessage(Observable_, {ServiceData::Command::Unsubscribe});
    Observable_ = nullptr;
  }

  bool isSubscribed() const {
    return Observable_ != nullptr;
  }

  CPointer address() {
    return this;
  }

private:
  void sendServiceMessage(CPointer to, ServiceData data) {
    QPort::send(this, to, CData(std::move(data)));
  }

  class MsgVisitor {
  public:
    MsgVisitor(CObserver* host, CPointer from) : host_(host), from_(from) {
    }

    void operator()(TData&& data) const {
      host_->handleData(std::move(data), from_);
    }

    void operator()(ServiceData data) const {
      host_->handleServiceData(data, from_);
    }

  private:
    CObserver* host_;
    CPointer from_;
  };

  friend class MsgVisitor;

  void handleData(TData&& data, CPointer from) const {
    if (!isSubscribed())
      return;
    if (Observable_ == from)
      NotifyCallback::onNotify(std::move(data));
  }

  void handleServiceData(ServiceData data, CPointer from) {
    switch (data.cmd) {
    case ServiceData::Command::Subscribe:
      if (isSubscribed() && Observable_ != from)
        sendServiceMessage(Observable_, {ServiceData::Command::Unsubscribe});
      Observable_ = from;
      break;
    case ServiceData::Command::Unsubscribe:
      if (Observable_ == from)
        Observable_ = nullptr;
      break;
    default:
      assert(false);
      break;
    }
  }

  void action(CPointer from, std::any&& data) override {
    CData msg = std::any_cast<CData>(std::move(data));
    std::visit(MsgVisitor(this, from), std::move(msg));
  }

  CPointer Observable_ = nullptr;
};

namespace detail {

template<class TData>
class CDataBind {
protected:
  using ServiceData = detail::ServiceData;
  using CData = std::variant<TData, ServiceData>;

private:
  using CReturn = const TData&;
  using CSignature = CReturn();
  using CGetAction = std::function<CSignature>;

protected:
  template<class TF>
  CDataBind(TF&& Data) : Data_(std::forward<TF>(Data)) {
    assert(Data_);
  }

  CData data() const {
    return Data_();
  }

private:
  CGetAction Data_;
};

template<>
class CDataBind<void> {
protected:
  using ServiceData = detail::ServiceData;
  using CData = std::variant<Empty, ServiceData>;

private:
  using CSignature = void();
  using CGetAction = std::function<CSignature>;

protected:
  CData data() const {
    return Empty{};
  }
};

template<class T>
using EnableIfVoid = std::enable_if_t<std::is_same_v<T, void>>;

template<class T>
using DisableIfVoid = std::enable_if_t<!std::is_same_v<T, void>>;

} // namespace detail

template<class TData>
class CObservable : protected QPort, protected detail::CDataBind<TData> {
  using DataBind = detail::CDataBind<TData>;
  using ServiceData = typename DataBind::ServiceData;
  using CData = typename DataBind::CData;
  using CObserver = CObserver<TData>;

public:
  using CPointer = QPort::CPointer;

private:
  using CListeners = std::list<CPointer>;

  friend CObserver;

public:
  template<class TF, class = detail::DisableIfVoid<TData>>
  CObservable(TF&& Data) : DataBind(std::forward<TF>(Data)) {
  }

  template<class = detail::EnableIfVoid<TData>>
  CObservable() {
  }

  CObservable(const CObservable&) = delete;
  CObservable(CObservable&&) noexcept = delete;
  CObservable& operator=(const CObservable&) = delete;
  CObservable& operator=(CObservable&&) noexcept = delete;
  ~CObservable() {
    unsubscribeAll();
  }

  void notify() {
    for (CPointer obs : Listeners_)
      sendData(obs);
  }

  void subscribe(CPointer obs) {
    if (obs == nullptr)
      return;
    if (!contains(obs)) {
      Listeners_.push_back(obs);
      sendServiceMessage(obs, {ServiceData::Command::Subscribe});
    }
    sendData(obs);
  }

  void unsubscribeAll() {
    while (!Listeners_.empty()) {
      sendServiceMessage(Listeners_.front(),
                         {ServiceData::Command::Unsubscribe});
      Listeners_.pop_front();
    }
  }

  CPointer address() {
    return this;
  }

private:
  void sendServiceMessage(CPointer to, ServiceData data) {
    QPort::send(this, to, CData(std::move(data)));
  }

  void sendData(CPointer to) {
    QPort::send(this, to, DataBind::data());
  }

  bool contains(CPointer obs) const {
    auto it = std::find(Listeners_.begin(), Listeners_.end(), obs);
    return it != Listeners_.end();
  }

  void unsubscribe(CPointer obs) {
    auto it = std::find(Listeners_.begin(), Listeners_.end(), obs);
    if (it != Listeners_.end()) {
      sendServiceMessage(*it, {ServiceData::Command::Unsubscribe});
      Listeners_.erase(it);
    }
  }

  class MsgVisitor {
  public:
    MsgVisitor(CObservable* host, CPointer from) : host_(host), from_(from) {
    }

    void operator()(TData&&) const {
      assert(false);
    }

    void operator()(ServiceData data) const {
      host_->handleServiceData(data, from_);
    }

  private:
    CObservable* host_;
    CPointer from_;
  };

  friend class MsgVisitor;

  void handleServiceData(ServiceData data, CPointer from) {
    switch (data.cmd) {
    case ServiceData::Command::Subscribe:
      subscribe(from);
      break;
    case ServiceData::Command::Unsubscribe:
      unsubscribe(from);
      break;
    default:
      assert(false);
      break;
    }
  }

  void action(CPointer from, std::any&& data) override {
    if (!contains(from))
      return;
    CData msg = std::any_cast<CData>(std::move(data));
    std::visit(MsgVisitor(this, from), std::move(msg));
  }

  CListeners Listeners_;
};

template<class TData>
class CObservableMono : protected CObservable<TData> {
  using CData = TData;
  using CBase = CObservable<CData>;
  using CObserver = CObserver<CData>;
  using CPointer = typename CBase::CPointer;

public:
  using CBase::CBase;

  using CBase::notify;

  void subscribe(CPointer obs) {
    CBase::unsubscribeAll();
    CBase::subscribe(obs);
  }
};

namespace detail {

template<class TData>
class CStorage {
  using CData = TData;

public:
  template<class... TArgs>
  explicit CStorage(TArgs&&... args) : Data_(std::forward<TArgs>(args)...) {
  }

protected:
  template<class... TArgs>
  void set(TArgs&&... args) {
    Data_ = CData(std::forward<TArgs>(args)...);
  }

  CData Data_{};
};

template<class TData, template<class T1> class TObservable>
class CObservableDataImpl : protected CStorage<TData>,
                            public TObservable<TData> {
  using CStorageBase = CStorage<TData>;
  using CObservableBase = TObservable<TData>;

public:
  template<class... TArgs>
  explicit CObservableDataImpl(TArgs&&... args)
      : CStorageBase(std::forward<TArgs>(args)...),
        CObservableBase(
            [&Data = CStorageBase::Data_]() -> const TData& { return Data; }) {
  }

  template<class... TArgs>
  void set(TArgs&&... args) {
    CStorageBase::set(std::forward<TArgs>(args)...);
    CObservableBase::notify();
  }
};
} // namespace detail

template<class TData>
using CObservableData = detail::CObservableDataImpl<TData, CObservable>;

template<class TData>
using CObservableDataMono = detail::CObservableDataImpl<TData, CObservableMono>;

} // namespace Library
} // namespace QApp
