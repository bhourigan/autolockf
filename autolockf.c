/* Brian Hourigan 2008 */

#include <stdio.h>
#include <libgen.h>
#include <fcntl.h>

char *lockf_path;
int lockf_fd = -1;

void cleanup(void){
   if(lockf_fd != -1){
      (void)close(lockf_fd);   
      (void)unlink(lockf_path);
   }
}

int main(int argc, char *argv[]){
   char *execstring;
   char *script;

   if(argc > 2)
      script = argv[argc - 1];
   else
      script = argv[1];

   asprintf(&lockf_path, "/tmp/%s.autolockf", basename(script));
   if((lockf_fd = open(lockf_path, O_RDWR|O_CREAT|O_NOFOLLOW|O_NONBLOCK, (mode_t)0600)) != -1){
      if(flock(lockf_fd, LOCK_EX) != -1){
         atexit(cleanup);

         asprintf(&execstring, "/bin/sh %s", script);
         system(execstring);
      } else
         fprintf(stdout, "autolockf: lock already acquired (flock)\n");
   } else
      fprintf(stdout, "autolockf: lock already acquired (open)\n");
}
