#pragma once

#include <QEvent>
#include <QPointer>

#include <any>

namespace QApp {
namespace Library {

namespace detail {

class Message : public QEvent {
  using CPointer = QPointer<QObject>;

public:
  static QEvent::Type type();

  Message(CPointer from, CPointer to, const std::any& data);
  Message(CPointer from, CPointer to, std::any&& data);

  std::any&& extract();

  bool isAlive() const;
  CPointer receiver() const;

  CPointer to() const;
  CPointer from() const;

private:
  CPointer from_;
  CPointer to_;
  std::any data_;
};

} // namespace detail

class QPort : public QObject {
  Q_OBJECT
  using Message = detail::Message;

protected:
  using CPointer = QPointer<QObject>;

public:
  void send(CPointer from, CPointer to, std::any data) const;
  bool event(QEvent* event) override;

protected:
  virtual void action(CPointer from, std::any&& data) = 0;

private:
  static constexpr bool k_is_processed = true;
};

} // namespace Library
} // namespace QApp
