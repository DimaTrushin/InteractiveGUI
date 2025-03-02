#include "QPorts.h"

#include <QApplication>

namespace QApp {
namespace Library {

namespace detail {

namespace {
bool isNumberCorrect(int number) {
  return number >= QEvent::User && number <= QEvent::MaxUser;
}
} // namespace
// namespace

QEvent::Type Message::type() {
  static int message_type = registerEventType();
  assert(isNumberCorrect(message_type));
  return QEvent::Type(message_type);
}

Message::Message(CPointer from, CPointer to, const std::any& data)
    : QEvent(type()), from_(std::move(from)), to_(std::move(to)), data_(data) {
}

Message::Message(CPointer from, CPointer to, std::any&& data)
    : QEvent(type()), from_(std::move(from)), to_(std::move(to)),
      data_(std::move(data)) {
}

std::any&& Message::extract() {
  return std::move(data_);
}

bool Message::isAlive() const {
  return to_;
}

Message::CPointer Message::receiver() const {
  return to_;
}

Message::CPointer Message::to() const {
  return to_;
}

Message::CPointer Message::from() const {
  return from_;
}
} // namespace detail

void QPort::send(CPointer from, CPointer to, std::any data) const {
  QCoreApplication::postEvent(
      QCoreApplication::instance(),
      new Message(std::move(from), std::move(to), std::move(data)));
}

bool QPort::event(QEvent* event) {
  if (event->type() == Message::type()) {
    Message* msg = static_cast<Message*>(event);
    action(msg->from(), msg->extract());
    return k_is_processed;
  }
  return QObject::event(event);
}

} // namespace Library
} // namespace QApp
