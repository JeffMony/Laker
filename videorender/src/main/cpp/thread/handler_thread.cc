//
// Created by jeffli on 2022/1/13.
//

#include "handler_thread.h"
#include "log.h"

namespace thread {

HandlerThread *HandlerThread::Create(std::string name) {
  return new HandlerThread(name);
}

static void *RunTask(void *context) {
  auto handler_thread = reinterpret_cast<HandlerThread *>(context);
  handler_thread->RunInternal();
  pthread_exit(0);
}

HandlerThread::HandlerThread(std::string name)
  : name_(name)
  , looper_(nullptr)
  , exiting_(false)
  , exited_(false) {
  pthread_mutex_init(&mutex_, nullptr);
  pthread_cond_init(&cond_, nullptr);
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  pthread_create(&thread_, &attr, RunTask, (void *) this);
  pthread_setname_np(thread_, name.c_str());
}

void HandlerThread::RunInternal() {
  pthread_mutex_lock(&mutex_);
  exiting_ = false;
  exited_ = false;
  pthread_mutex_unlock(&mutex_);

  Looper::Prepare();
  pthread_mutex_lock(&mutex_);
  looper_ = Looper::MyLooper();
  pthread_cond_broadcast(&cond_);
  pthread_mutex_unlock(&mutex_);

  Looper::Loop();
  Looper::Exit();

  pthread_mutex_lock(&mutex_);
  if (looper_ == nullptr) {
    exited_ = true;
    pthread_mutex_unlock(&mutex_);
    return;
  }

  exiting_ = false;
  if (looper_) {
    delete looper_;
    looper_ = nullptr;
  }
  exited_ = true;
  pthread_mutex_unlock(&mutex_);
}

HandlerThread::~HandlerThread() {
  LOGI("Enter: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  if (looper_) {
    delete looper_;
    looper_ = nullptr;
  }
  pthread_mutex_destroy(&mutex_);
  pthread_cond_destroy(&cond_);
  LOGI("Leave: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
}

void HandlerThread::Quit() {
  LOGI("Enter: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  pthread_mutex_lock(&mutex_);
  if (exiting_ || exited_) {
    pthread_mutex_unlock(&mutex_);
    return;
  }
  exiting_ = true;
  pthread_mutex_unlock(&mutex_);
  Looper *looper = GetLooper();
  if (looper) {
    looper->Quit(false);
  }
  pthread_detach(thread_);
  LOGI("Leave: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
}

bool HandlerThread::QuitSafely() {
  LOGI("Enter: %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  pthread_mutex_lock(&mutex_);
  if (exiting_ || exited_) {
    pthread_mutex_unlock(&mutex_);
    pthread_detach(thread_);
    return false;
  }
  exiting_ = true;
  pthread_mutex_unlock(&mutex_);
  Looper *looper = GetLooper();
  if (looper) {
    looper->Quit(true);
    pthread_join(thread_, 0);
    LOGI("Leave 1 %s %s %d", __FILE_NAME__, __func__ , __LINE__);
    return true;
  }
  pthread_detach(thread_);
  LOGI("Leave 2 %s %s %d", __FILE_NAME__, __func__ , __LINE__);
  return false;
}

Looper *HandlerThread::GetLooper() {
  pthread_mutex_lock(&mutex_);
  if (exited_) {
    LOGE("%s %s %d Thread has been exited", __FILE_NAME__, __func__ , __LINE__);
    pthread_mutex_unlock(&mutex_);
    return nullptr;
  }
  if (looper_ == nullptr) {
    LOGI("%s %s %d Thread should wait", __FILE_NAME__, __func__ , __LINE__);
    pthread_cond_wait(&cond_, &mutex_);
  }
  pthread_mutex_unlock(&mutex_);
  return looper_;
}

}