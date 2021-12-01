#include <chrono>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include <ftw.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>

void split(char const * str, std::vector<char *> & res, char c = ' ');

int main (int argc, char ** argv)
{
        std::string current_command;
        std::vector<char *> split_res;
        std::cout << '>';
        while(std::getline(std::cin, current_command))
        {
                std::cout << '>';
                split_res.clear();
                split(current_command.c_str(), split_res);
                int pid = fork();
				// if something goes wrong
				if (pid < 0)
				{
					perror(NULL);
					exit(-1);
				}
                if(pid == 0)
                {
                        std::string stdout_filename = "/opt/silentshell/" + std::to_string(getpid()) + "/out.std";
                        std::string stderr_filename = "/opt/silentshell/" + std::to_string(getpid()) + "/err.std";
                        std::string stdin_filename = "/opt/silentshell/" + std::to_string(getpid()) + "/in.std";
                       //file about output messages
						int out_fd = open(stdout_filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
						// if something goes wrong
						if (out_fd < 0)
						{
							perror(NULL);
							exit(-1);
					    }
						//file about error messages 
                        int err_fd = open(stderr_filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
						// if something goes wrong
					    if (out_fd < 0)
					    {
							perror(NULL);
							exit(-1);
					    }
						//file about input messages
                        int in_fd = open(stdin_filename.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                        //if someting goes wrong
						if(in_fd < 0)
                        int out_dup = dup2(out_fd, STDOUT_FILENO);
						if (out_dup < 0)
						{
							perror(NULL);
							exit(-1);
						}
						//file for holding output information
                        int err_dup = dup2(out_fd, STDERR_FILENO);
						//if something goes wrong
						if (err_dup < 0)
						{
							perror(NULL);
							exit(-1);
						}
						//file for holding input information
                        int in_dup = dup2(in_fd, STDIN_FILENO);
						//if something goes wrong
						if (in_dup < 0)
						{
							perror(NULL);
							exit(-1);
						}
						//converting from vector to char**
                        std::unique_ptr<char *> params(new char  * [split_res.size()]);
                        for(size_t i = 0; i < split_res.size(); i++)
                        {
                                params.get()[i] = split_res[i];
                        }
                        int exec_res = execvp(params.get()[0], params.get());
						if (exec_res < 0)
						{
							perror(NULL);
							exit(-1);
					    }
                        for(size_t i = 0; i < split_res.size(); i++)                       
						{
                                delete[] split_res[i];
                        }
                        exit(EXIT_SUCCESS);
                }
                else
                        wait(NULL);
        }

}

// separation by spaces
void split(char const * str, std::vector<char *>& res, char c)
{
        do
        {
                char const * begin = str;
                while(*str != c && *str)
                        ++str;
                char * temp = new char[str - begin + 1];
                memcpy(temp, begin, str - begin);
                temp[str - begin] = 0;
                res.push_back(temp);
        } while(0 != *str++);
        res.push_back((char*)NULL);
}
