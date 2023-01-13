//
// Created by jeffli on 2022/1/13.
//

#include "message_queue.h"
#include "log.h"
#include <sstream>

namespace thread {

MessageQueue::MessageQueue()
  : is_destroyed_(false) {
  pthread_mutex_init(&queue_mutex_, nullptr);
  pthread_cond_init(&queue_cond_, nullptr);
}

MessageQueue::~MessageQueue() {
  LOGI("Enter: %s %s %d is_destroyed_=%d", __FILE_NAME__, __func__ , __LINE__, is_destroyed_);
  if (is_destroyed_) {
    return;
  }
  pthread_mutex_lock(&queue_mutex_);
  is_destroyed_ = true;
  pthread_mutex_unlock(&queue_mutex_);

  pthread_mutex_destroy(&queue_mutex_);
  pthread_cond_destroy(&queue_cond_);
  LOGI("Leave: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
}

void MessageQueue::Offer(Message *msg) {
  if (is_destroyed_) {
    return;
  }
  pthread_mutex_lock(&queue_mutex_);
  queue_.push_back(msg);
  pthread_cond_broadcast(&queue_cond_);
  pthread_mutex_unlock(&queue_mutex_);
}

void MessageQueue::OfferAtFront(Message *msg) {
  if (is_destroyed_) {
    return;
  }
  pthread_mutex_lock(&queue_mutex_);
  queue_.push_front(msg);
  pthread_cond_broadcast(&queue_cond_);
  pthread_mutex_unlock(&queue_mutex_);
}

Message *MessageQueue::Take() {
  if (is_destroyed_) {
    return nullptr;
  }
  pthread_mutex_lock(&queue_mutex_);
  if (Size() <= 0) {
//    LOGV("Message queue is empty, should wait");
    pthread_cond_wait(&queue_cond_, &queue_mutex_);
//    LOGV("Message queue continues to work");
  }
  if (queue_.empty()) {
    pthread_mutex_unlock(&queue_mutex_);
    return nullptr;
  }
  Message *msg = queue_.front();
  queue_.pop_front();
  pthread_mutex_unlock(&queue_mutex_);
  return msg;
}

void MessageQueue::Notify() {
  pthread_mutex_lock(&queue_mutex_);
  pthread_cond_broadcast(&queue_cond_);
  pthread_mutex_unlock(&queue_mutex_);
}

int MessageQueue::Size() {
  return queue_.size();
}

bool MessageQueue::IsEmpty() {
  return queue_.empty();
}

void MessageQueue::Clear() {
  Notify();
  if (queue_.empty()) {
    return;
  }
  pthread_mutex_lock(&queue_mutex_);
  while (!queue_.empty()) {
    Message *msg = queue_.front();
    queue_.pop_front();
    if (msg) {
      delete msg;
    }
  }
  queue_.clear();
  pthread_mutex_unlock(&queue_mutex_);
}

void MessageQueue::RemoveMessage(int what) {
  if (is_destroyed_) {
    return;
  }
  pthread_mutex_lock(&queue_mutex_);
  std::list<Message *>::iterator it = queue_.begin();
  while (it != queue_.end()) {
    Message *msg = *it;
    if (what == msg->what) {
      delete msg;
      it = queue_.erase(it);
      continue;
    }
    ++it;
  }
  pthread_mutex_unlock(&queue_mutex_);
}

void MessageQueue::Dump() {
  if (is_destroyed_) {
    return;
  }
  pthread_mutex_lock(&queue_mutex_);
  std::ostringstream os;
  std::list<Message *>::iterator it = queue_.begin();
  while (it != queue_.end()) {
    Message *msg = *it;
    os << msg->what<<"\n";
    ++it;
  }
  LOGI("%s %s %d MessageQueue Result=%s", __FILE_NAME__, __func__ , __LINE__, os.str().c_str());
  pthread_mutex_unlock(&queue_mutex_);
}

}