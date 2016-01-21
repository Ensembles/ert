#include <spawn.h>

/**
    A small function used to redirect a file descriptior,
    only used as a helper utility for util_fork_exec().
*/

//static void __util_redirect(int src_fd , const char * target_file , int open_flags) {
//  int new_fd = open(target_file , open_flags , 0644);
//  dup2(new_fd , src_fd);
//  close(new_fd);
//}

///**
//   This function does the following:
//
//    1. Fork current process.
//    2. if (run_path != NULL) chdir(run_path)
//    3. The child execs() to run executable.
//    4. Parent can wait (blocking = true) on the child to complete executable.
//
//   If the executable is an absolute path it will run the command with
//   execv(), otherwise it will use execvp() which will (try) to look up
//   the executable with the PATH variable.
//
//   argc / argv are the number of arguments and their value to the
//   external executable. Observe that prior to calling execv the argv
//   list is prepended with the name of the executable (convention), and
//   a NULL pointer is appended (requirement by execv).
//
//   If stdout_file != NULL stdout is redirected to this file.  Same
//   with stdin_file and stderr_file.
//
//   If target_file != NULL, the parent will check that the target_file
//   has been created before returning; and abort if not. In this case
//   you *MUST* have blocking == true, otherwise it will abort on
//   internal error.
//
//
//   The return value from the function is the pid of the child process;
//   this is (obviously ?) only interesting if the blocking argument is
//   'false'.
//
//   Example:
//   --------
//   util_spawn("/local/gnu/bin/ls" , 1 , (const char *[1]) {"-l"} ,
//   "listing" , NULL, NULL);
//
//
//   This program will run the command 'ls', with the argument '-l'. The
//   main process will block, i.e. wait until the 'ls' process is
//   complete, and the results of the 'ls' operation will be stored in
//   the file "listing". If the 'ls' should want to print something on
//   stderr, it will go there, as stderr is not redirected.
//
//*/
//

static posix_spawn_file_actions_t * __create_redirection(const char *stdout_file, const char *stderr_file) {
  posix_spawn_file_actions_t * action = malloc(sizeof action);
  if(posix_spawn_file_actions_init(action) != 0) {
    util_abort("%s: Failed to initialize file actions!\n", __func__);
  }

  if (posix_spawn_file_actions_addclose(action, 0) != 0) {
    util_abort("%s: Unable to close STDIN. Aborting!\n", __func__);
  }

  if(stdout_file != NULL) {
    if (posix_spawn_file_actions_addclose(action, 1) != 0) {
      util_abort("%s: Unable to close STDOUT. Aborting!\n", __func__);
    }

//    int fd = open(stdout_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
//
//    if (fd < 0) {
//      util_abort("%s: Unable to open file: %s to redirect STDOUT to. Aborting!\n", __func__, stdout_file);
//    }

    if (posix_spawn_file_actions_addopen(action, 1, stdout_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR) != 0) {
      util_abort("%s: Unable to redirect STDOUT to: %s Aborting!\n", __func__, stdout_file);
    }
//    if (posix_spawn_file_actions_adddup2(action, fd, 1) != 0) {
//      util_abort("%s: Unable to redirect STDOUT to: %s Aborting!\n", __func__, stdout_file);
//    }
//
//    if (posix_spawn_file_actions_addclose(action, fd) != 0) {
//      util_abort("%s: Unable to redirect STDOUT to: %s Aborting!\n", __func__, stdout_file);
//    }
  }

  if(stderr_file != NULL) {
    if (posix_spawn_file_actions_addclose(action, 2) != 0) {
      util_abort("%s: Unable to close STDERR. Aborting!\n", __func__);
    }

    if (posix_spawn_file_actions_addopen(action, 2, stderr_file, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR) != 0) {
      util_abort("%s: Unable to redirect STDERR to: %s Aborting!\n", __func__, stderr_file);
    }
  }

  return action;
}


int util_spawn(const char *executable, int argc, const char **argv, const char *stdout_file, const char *stderr_file, pid_t *pid_holder) {
  char **__argv = util_malloc((argc + 2) * sizeof *__argv);
  __argv[0] = (char*) executable;
  int iarg;
  for (iarg = 0; iarg < argc; iarg++)
    __argv[iarg + 1] = (char*) argv[iarg];
  __argv[argc + 1] = NULL;

  posix_spawn_file_actions_t * action = NULL;
  if(stdout_file != NULL || stderr_file != NULL) {
//    action = __create_redirection(stdout_file, stderr_file);
  }

  extern char **environ;
  pid_t pid;

  if (pid_holder == NULL) {
    pid_holder = &pid;
  }

  int status = posix_spawn(pid_holder, executable, action, NULL, __argv, environ);

  if (status != 0) {
    util_abort("%s: failed to execute external command: \'%s\': %s \n", __func__, executable, strerror(status));
  }

  if (waitpid(pid, &status, 0) == -1) {
    util_abort("%s: spawned process failed: \'%s\': %s \n", __func__, executable);
  }

  printf("--> %d\n", status);

  if(action != NULL) {
    posix_spawn_file_actions_destroy(action);
  }
  util_safe_free(__argv);

  return status;
}


/**
   The ping program must(?) be setuid root, so implementing a simple
   version based on sockets() proved to be nontrivial.

   The PING_CMD is passed as -D from the build system.
*/

#ifdef ERT_HAVE_PING
bool util_ping(const char *hostname) {
  int wait_status = util_spawn(PING_CMD, 4, (const char *[4]) {"-c" , "3" , "-q", hostname}, "/dev/null" , "/dev/null", NULL);

  if (WIFEXITED( wait_status )) {
      int ping_status = WEXITSTATUS( wait_status );
      return ping_status == 0;
  } else {
      return false;
  }
}
#endif




