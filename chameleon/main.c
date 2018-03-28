#include <stdio.h>
#include <stdlib.h> 
/** Chameleon that you can use to mimic any application that :
      - accept input parameters
      - write something on stdout
      - return error code
    */

/** Chameleon reserves argv input for test purpose. To give it
    commands you have to use environment variables. All of them are optional.
    Each of them activate a specific feature.*/
const char * const env_command[] =
  {
    "CHAMELEON_INTERNAL_ERROR", /** It is probably the most special. By default internal application error return 1*/
    "CHAMELEON_RETURN",         /** Returned error code */
    "CHAMELEON_ARGV",           /** File to log provided cli input parameters */
    "CHAMELEON_OUT"             /** File that store what you like that application print out */
};


char * readline(FILE *fp, char *buffer)
{
  int ch;
  int i = 0;
  size_t buff_len = 0;
  void *tmp;

  buffer = malloc(buff_len + 1);
  if (!buffer)
  {
    return NULL;
  }

  while ((ch = fgetc(fp)) != '\n' && ch != EOF)
  {
    buff_len++;
        
    tmp = realloc(buffer, buff_len + 1);
    if (tmp == NULL)
    {
        free(buffer);
        return NULL;
    }
    buffer = tmp;

    buffer[i] = (char) ch;
    i++;
  }
  buffer[i] = '\0';

  // Detect end
  if (ch == EOF && (i == 0 || ferror(fp)))
  {
    free(buffer);
    return NULL;
  }
  return buffer;
}

int validate_return(char *in, int out_default)
{
  int return_default = out_default;
  if((NULL != in) && (in[0] >= '0')&&(in[0] <= '9'))
  {
    return_default = in[0] - '0';
  }
  return return_default;
}

int main( int argc, char *argv[] )
{
  int i=0;
  char * chameleon_internal_error_env; /** env var if user want to overwrite default error value for internal errors (that is 1) */
  int chameleon_internal_error = 1;
  char * chameleon_return_env; /** env var if user want to overwrite default error value for internal errors (that is 1) */
  int chameleon_return = 0;  /** default return value, can be controlled by ENV CHAMELEON_RETURN*/
  char * chameleon_argv; /** it store file name where chameleon will write input params*/
  char * chameleon_out;  /** it store file name from where chameleon will read messages to print to stdout*/
  FILE *chameleon_argv_file;
  FILE *chameleon_out_file;

  char *stdout_line;

  chameleon_internal_error_env = getenv(env_command[0]);
  chameleon_internal_error = validate_return(chameleon_internal_error_env, chameleon_internal_error);

  chameleon_return_env = getenv(env_command[1]);
  chameleon_return = validate_return(chameleon_return_env, chameleon_return);

  /* is user set CHAMELEON_ARGV env variable it activates argument log feature*/
  chameleon_argv = getenv(env_command[2]);
  if(NULL != chameleon_argv)
  {
    chameleon_argv_file = fopen(chameleon_argv, "w");
    /* If feature is active it has to be possible to open file to use it
     otherwise fails */
    if(NULL == chameleon_argv_file)
    {
      return chameleon_internal_error;
    }

    fprintf(chameleon_argv_file, "argc=%d\n",argc);
    for (i=0;i<argc;i++)
    {
      fprintf(chameleon_argv_file, "argv[%d]=%s\n",i,argv[i]);
    }
    fclose(chameleon_argv_file);
  }

  /* is user set CHAMELEON_OUT env variable it activates stout generation feature*/
  chameleon_out = getenv(env_command[3]);
  if(NULL != chameleon_out)
  {
    chameleon_out_file = fopen(chameleon_out, "r");
    /* if user activated this feature it has to provide readable file to print */
    if(NULL == chameleon_out_file)
    {
      return chameleon_internal_error;
    }

    while (NULL != (stdout_line = readline(chameleon_out_file, 0)))
    {
        puts(stdout_line);
        free(stdout_line);
        //printf("\n");
    }
    fclose(chameleon_out_file);
    
  }
 
  return chameleon_return;
}