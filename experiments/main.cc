#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

class Lock {
 public:
  Lock(pthread_mutex_t* mutex) : mutex_(mutex) {
    pthread_mutex_lock(mutex_);
  }

  ~Lock() {
    pthread_mutex_unlock(mutex_);
  }

 private:
  pthread_mutex_t* mutex_;
  Lock(const Lock& no_copy);
  Lock& operator=(const Lock& no_assign);
};

class BackgroundThread {
 public:
  static BackgroundThread* Create() {
    BackgroundThread* thread = new BackgroundThread();
    int status = thread->CreateThread();
    if (status != 0) {
      delete thread;
      return NULL;
    }
    return thread;
  }

  void ForgetThread() {
    join_ = false;
  }

  ~BackgroundThread() {
    if (join_) {
      pthread_join(thread_, NULL);
    }
    pthread_cond_destroy(&cond_);
    pthread_mutex_destroy(&mutex_);
  }

 private:
  void Run() {
    for (int i = 0; i < 25; ++i) {
      printf("BackgroundThread::Run(): pid=%d, tid=%ld\n",
        getpid(), pthread_self());
      sleep(1);
    }
  }

  static void* ThreadFunction(void* arg) {
    // Disable all signals
    sigset_t mask_all={0};
    sigfillset(&mask_all);
    pthread_sigmask(SIG_BLOCK, &mask_all, NULL);

    // Cast argument to class instance and call Run()
    BackgroundThread* thread = reinterpret_cast<BackgroundThread*>(arg);
    thread->Run();
    return NULL;
  }

  int CreateThread() {
    int status = pthread_create(&thread_, NULL, ThreadFunction, this);
    return status;
  }

  BackgroundThread() : thread_(0), join_(true) {
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&cond_, NULL);
  }

  BackgroundThread(const BackgroundThread& no_copy);
  BackgroundThread& operator=(const BackgroundThread& no_assign);
  pthread_mutex_t mutex_;
  pthread_cond_t cond_;
  pthread_t thread_;
  bool join_;
};

pthread_mutex_t GlobalMutex = PTHREAD_MUTEX_INITIALIZER;
bool GlobalInit = false;
BackgroundThread* GlobalThread = NULL;

void identify(const char* who) {
  printf("pid=%d, who=%s\n", getpid(), who);
}

void prepare() {
  identify("prepare");
  pthread_mutex_lock(&GlobalMutex);
}

void parent() {
  pthread_mutex_unlock(&GlobalMutex);
  identify("parent");
}

void child() {
  GlobalThread->ForgetThread();
  delete GlobalThread;
  GlobalThread = BackgroundThread::Create();
  pthread_mutex_unlock(&GlobalMutex);
  identify("child");
}

int library_init() {
  identify("library_init");
  Lock lock(&GlobalMutex);
  if (!GlobalInit) {
    int status = pthread_atfork(prepare, parent, child);
    (void)status;
    GlobalThread = BackgroundThread::Create();
    GlobalInit = true;
  }
  return 0;
}

int library_shutdown() {
  Lock lock(&GlobalMutex);
  identify("library_shutdown");
  delete GlobalThread;
  GlobalThread = NULL;
  GlobalInit = false;
  return 0;
}

int main(int argc, char* argv[]) {
  library_init();
  fork();
  library_shutdown();
  return 0;
}

