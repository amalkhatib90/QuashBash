/**
 * @file execute.c
 *
 * @brief Implements interface functions between Quash and the environment and
 * functions that interpret an execute commands.
 *
 * @note As you add things to this file you may want to change the method signature
 */

#include "execute.h"

#include <stdio.h>

#include "quash.h"

#include <unistd.h>

#include <stdlib.h>

#include "deque.h"
// Remove this and all expansion calls to it
/**
 * @brief Note calls to any function that requires implementation
 */
#define IMPLEMENT_ME()                                                  \
  fprintf(stderr, "IMPLEMENT ME: %s(line %d): %s()\n", __FILE__, __LINE__, __FUNCTION__)

//sets up deque struct for processes
IMPLEMENT_DEQUE_STRUCT(PIDDeque, pid_t);
//sets up deque functions
IMPLEMENT_DEQUE(PIDDeque, pid_t);

typedef struct job
{
  PIDDeque process_list;
  int job_id;
  char* cmd;
}job;

//jobs contructor
struct job _new_job()
{
  return(job){
    0,
    get_command_string(),
    new_PIDDeque,
  };
}

//needs work
//implement destructor, free process command and destroy process deque
// struct job _destroy_job()
// {
//   free(cmd);
//   need help destroying deque
//   PIDDeque new_destructable_PIDDeque(//need size,pid_t)
// }
//sets up job deque
IMPLEMENT_DEQUE_STRUCT(JOBDeque, job);
//sets up functions for deque
IMPLEMENT_DEQUE(JOBDeque, job);
//declares job deque, needs to initalize in the start of run_script
JOBDeque jobs;

//delcaring pipes
int pipes[2][2];
//ex: pipes[0][1] write end of pipe 0
//keeps track of which pipe we are using and %2 it to see which pipe we need to use
int pipeUsed = 0;
/***************************************************************************
 * Interface Functions
 ***************************************************************************/

// Return a string containing the current working directory.
char* get_current_directory(bool* should_free) {
  // TODO: Get the current working directory. This will fix the prompt path.
  // HINT: This should be pretty simple
  //IMPLEMENT_ME();
  char* dbuff = NULL;
  dbuff = getcwd(NULL, 0);
  // Change this to true if necessary
  *should_free = true;

  return dbuff;
}

// Returns the value of an environment variable env_var
const char* lookup_env(const char* env_var) {
  // TODO: Lookup environment variables. This is required for parser to be able
  // to interpret variables from the command line and display the prompt
  // correctly
  // HINT: This should be pretty simple
  //IMPLEMENT_ME();
  printf("%s/", env_var);
  // TODO: Remove warning silencers
  //(void) env_var; // Silence unused variable warning

  return getenv(env_var);
}

// Check the status of background jobs
void check_jobs_bg_status() {
  // TODO: Check on the statuses of all processes belonging to all background
  // jobs. This function should remove jobs from the jobs queue once all
  // processes belonging to a job have completed.
  IMPLEMENT_ME();

  // TODO: Once jobs are implemented, uncomment and fill the following line
  // print_job_bg_complete(job_id, pid, cmd);
}

// Prints the job id number, the process id of the first process belonging to
// the Job, and the command string associated with this job
void print_job(int job_id, pid_t pid, const char* cmd) {
  printf("[%d]\t%8d\t%s\n", job_id, pid, cmd);
  fflush(stdout);
}

// Prints a start up message for background processes
void print_job_bg_start(int job_id, pid_t pid, const char* cmd) {
  printf("Background job started: ");
  print_job(job_id, pid, cmd);
}

// Prints a completion message followed by the print job
void print_job_bg_complete(int job_id, pid_t pid, const char* cmd) {
  printf("Completed: \t");
  print_job(job_id, pid, cmd);
}

/***************************************************************************
 * Functions to process commands
 ***************************************************************************/
// Run a program reachable by the path environment variable, relative path, or
// absolute path
void run_generic(GenericCommand cmd) {
  // Execute a program with a list of arguments. The `args` array is a NULL
  // terminated (last string is always NULL) list of strings. The first element
  // in the array is the executable
  char* exec = cmd.args[0];
  char** args = cmd.args;

  // TODO: Remove warning silencers
  execvp(exec, args); // Silence unused variable warning

  // TODO: Implement run generic


  perror("ERROR: Failed to execute program");
}

// Print strings
void run_echo(EchoCommand cmd) {
  // Print an array of strings. The args array is a NULL terminated (last
  // string is always NULL) list of strings.
  char** str = cmd.args;

  // TODO: Remove warning silencers
  //(void) str; // Silence unused variable warning

  // TODO: Implement echo
  //IMPLEMENT_ME();
  printf("%s\n", str);
  // Flush the buffer before returning
  fflush(stdout);
}

// Sets an environment variable
void run_export(ExportCommand cmd) {
  // Write an environment variable
  const char* env_var = cmd.env_var;
  const char* val = cmd.val;

  // TODO: Remove warning silencers
  //(void) env_var; // Silence unused variable warning
  //(void) val;     // Silence unused variable warning

  // TODO: Implement export.
  // HINT: This should be quite simple.
  //IMPLEMENT_ME();
  //changes env_var value to val and since parameter 3 is 1 it overwrites it
  setenv(env_var, val, 1);
}

// Changes the current working directory
void run_cd(CDCommand cmd) {
  // Get the directory name
  const char* dir = cmd.dir;

  // Check if the directory is valid
  if (dir == NULL) {
    perror("ERROR: Failed to resolve path");
    return;
  }
  else{
    get_current_directory(dir);
    char* env_var = get_current_directory(dir);
    lookup_env(env_var);
    //OLD_PWD = dir;

  }

  // TODO: Change directory

  // TODO: Update the PWD environment variable to be the new current working
  // directory and optionally update OLD_PWD environment variable to be the old
  // working directory.
  IMPLEMENT_ME();
}

// Sends a signal to all processes contained in a job
void run_kill(KillCommand cmd) {
  int signal = cmd.sig;
  int job_id = cmd.job;

  // TODO: Remove warning silencers
  (void) signal; // Silence unused variable warning
  (void) job_id; // Silence unused variable warning

  // TODO: Kill all processes associated with a background job
  IMPLEMENT_ME();
}


// Prints the current working directory to stdout
void run_pwd() {
  // TODO: Print the current working directory
  //IMPLEMENT_ME();
  //used get current directory and free it
  bool* should_free = false;
  char* dbuff = get_current_directory(should_free);
  printf("%s", dbuff);
  free(dbuff);
  // Flush the buffer before returning
  fflush(stdout);
}

// Prints all background jobs currently in the job list to stdout
void run_jobs() {
  // TODO: Print background jobs
  IMPLEMENT_ME();

  // Flush the buffer before returning
  fflush(stdout);
}

/***************************************************************************
 * Functions for command resolution and process setup
 ***************************************************************************/

/**
 * @brief A dispatch function to resolve the correct @a Command variant
 * function for child processes.
 *
 * This version of the function is tailored to commands that should be run in
 * the child process of a fork.
 *
 * @param cmd The Command to try to run
 *
 * @sa Command
 */
void child_run_command(Command cmd) {
  CommandType type = get_command_type(cmd);

  switch (type) {
  case GENERIC:
    run_generic(cmd.generic);
    break;

  case ECHO:
    run_echo(cmd.echo);
    break;

  case PWD:
    run_pwd();
    break;

  case JOBS:
    run_jobs();
    break;

  case EXPORT:
  case CD:
  case KILL:
  case EXIT:
  case EOC:
    break;

  default:
    fprintf(stderr, "Unknown command type: %d\n", type);
  }
}

/**
 * @brief A dispatch function to resolve the correct @a Command variant
 * function for the quash process.
 *
 * This version of the function is tailored to commands that should be run in
 * the parent process (quash).
 *
 * @param cmd The Command to try to run
 *
 * @sa Command
 */
void parent_run_command(Command cmd) {
  CommandType type = get_command_type(cmd);

  switch (type) {
  case EXPORT:
    run_export(cmd.export);
    break;

  case CD:
    run_cd(cmd.cd);
    break;

  case KILL:
    run_kill(cmd.kill);
    break;

  case GENERIC:
  case ECHO:
  case PWD:
  case JOBS:
  case EXIT:
  case EOC:
    break;

  default:
    fprintf(stderr, "Unknown command type: %d\n", type);
  }
}

/**
 * @brief Creates one new process centered around the @a Command in the @a
 * CommandHolder setting up redirects and pipes where needed
 *
 * @note Processes are not the same as jobs. A single job can have multiple
 * processes running under it. This function creates a process that is part of a
 * larger job.
 *
 * @note Not all commands should be run in the child process. A few need to
 * change the quash process in some way
 *
 * @param holder The CommandHolder to try to run
 *
 * @sa Command CommandHolder
 */
void create_process(CommandHolder holder) {
  // Read the flags field from the parser
  bool p_in  = holder.flags & PIPE_IN;
  bool p_out = holder.flags & PIPE_OUT;
  bool r_in  = holder.flags & REDIRECT_IN;
  bool r_out = holder.flags & REDIRECT_OUT;
  bool r_app = holder.flags & REDIRECT_APPEND; // This can only be true if r_out
                                               // is true

  // // TODO: Remove warning silencers
  // (void) p_in;  // Silence unused variable warning
  // (void) p_out; // Silence unused variable warning
  // (void) r_in;  // Silence unused variable warning
  // (void) r_out; // Silence unused variable warning
  // (void) r_app; // Silence unused variable warning

  // TODO: Setup pipes, redirects, and new process
  //IMPLEMENT_ME();

  //attempt 1
  // if(pid_t > 0)
  // {
  //   if(p_in == false)
  //   {
  //     pipes[0][0] = holder.cmd;
  //     p_in = true;
  //   }
  //   else
  //   {
  //       pipes[1][0] = holder.cmd;
  //       p_in = false;
  //   }
  // }
  // if(pid_t == 0)
  // {
  //   if(p_out == false)
  //   {
  //     holder.cmd = pipes[0][1];
  //     p_out = true;
  //   }
  //   else
  //   {
  //     holder.cmd = pipes[1][1];
  //     p_out = false;
  //   }
  // }

  

  parent_run_command(holder.cmd); // This should be done in the parent branch of
                                  // a fork
  child_run_command(holder.cmd); // This should be done in the child branch of a fork
}

// Run a list of commands
void run_script(CommandHolder* holders) {
  if (holders == NULL)
    return;

  check_jobs_bg_status();

  if (get_command_holder_type(holders[0]) == EXIT &&
      get_command_holder_type(holders[1]) == EOC) {
    end_main_loop();
    return;
  }

  CommandType type;

  // Run all commands in the `holder` array
  for (int i = 0; (type = get_command_holder_type(holders[i])) != EOC; ++i)
    create_process(holders[i]);

  if (!(holders[0].flags & BACKGROUND)) {
    // Not a background Job
    // TODO: Wait for all processes under the job to complete
    IMPLEMENT_ME();
  }
  else {
    // A background job.
    // TODO: Push the new job to the job queue
    //figure out new job ID and push it to the back of the job queue and print that the background job has started
    IMPLEMENT_ME();

    // TODO: Once jobs are implemented, uncomment and fill the following line
    // print_job_bg_start(job_id, pid, cmd);
  }
}
