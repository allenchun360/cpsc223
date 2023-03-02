#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define Error (stderr)

enum state {code, slash, string, character, bracket, beginlc, beginmc, lcomment, mcomment, singletag, multitag, asterisk, asterisk2, tagasterisk};

int main(int argc, char *argv[])
{
    bool input = false;
    int indexi;
    bool output = false;
    int indexo;
    bool dasha = false;
    int indexa;
    bool dashl = false;
    int indexl;


    for(int i = 0; i < argc - 1; i++)
    {
        if(strcmp(argv[i], "-i") == 0)
        {
            input = true;
            indexi = i;
        }
        else if(strcmp(argv[i], "-o") == 0)
        {
            output = true;
            indexo = i;
        }
        else if(strcmp(argv[1], "-a") == 0)
        {
            dasha = true;
            indexa = i;
        }
        else if(strcmp(argv[i], "-l") == 0)
        {
            dashl = true;
            indexl = i;
        }
    }


    char s[10000];


    if (!input && !output)
    {

        if(dasha)
        {
            printf("yay");
            enum state curr = code;
            while (fgets(s, 1000, stdin) != NULL)
            {
                    if (curr == lcomment)
                    {
                        curr = code;
                    }

                    if (curr == mcomment)
                    {
                        curr = mcomment;
                    }

                    if (curr == code)
                    {
                        curr = code;
                    }

                    if (curr == slash)
                    {
                        curr = slash;
                    }

                    if (curr == string)
                    {
                        curr = string;
                    }

                    if (curr == character)
                    {
                        curr = character;
                    }

                    if (curr == bracket)
                    {
                        curr = bracket;
                    }

                    if (curr == singletag)
                    {
                        fprintf(stdout, "\n");
                        curr = code;
                    }

                    if (curr == multitag)
                    {
                        fprintf(stdout, "\n");
                        curr = mcomment;
                    }

                    if (curr == tagasterisk)
                    {
                        fprintf(stdout, "\n");
                        curr = mcomment;
                    }

                    if (curr == asterisk)
                    {
                        curr = mcomment;
                    }

                    if (curr == asterisk2)
                    {
                        curr = asterisk2;
                    }

                    if (curr == beginmc)
                    {
                        curr = beginmc;
                    }

                    if (curr == beginlc)
                    {
                        curr = beginlc;
                    }
                }

                for (int i = 0; s[i] != '\0'; i++)
                {
                    switch (curr)
                    {
                        case code:
                        {
                            if(s[i] == '/')
                            {
                                curr = slash;
                            }
                            else if(s[i] == '"')
                            {
                                curr = string;
                            }
                            else if(s[i] == '\'')
                            {
                                curr = character;
                            }
                            else if(s[i] == '{')
                            {
                                curr = bracket;
                            }
                            else
                            {
                                curr = code;
                            }
                            break;
                        }

                        case slash:
                        {
                            if(s[i] == '/')
                            {
                                curr = lcomment;
                            }
                            else if(s[i] == '*')
                            {
                                curr = mcomment;
                            }
                            else
                            {
                                curr = code;
                            }
                            break;
                        }

                        case string:
                        {
                            if(s[i] == '"')
                            {
                                curr = code;
                            }
                            break;
                        }

                        case character:
                        {
                            if(s[i] == '\'')
                            {
                                curr = code;
                            }
                            break;
                        }

                        case bracket:
                        {
                            if(s[i] == '}')
                            {
                                curr = code;
                            }
                            break;
                        }

                        case lcomment:
                        {
                            if(s[i] == '@' && (s[i - 1] == ' ' || (s[i - 1] == '/' && s[i - 2] == '/')))
                            {
                                curr = singletag;
                                fprintf(stdout, "%c", s[i]);
                            } 
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == ' ')
                            {
                                curr = lcomment;
                                fprintf(stdout, "\n");
                            }
                            else
                            {
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }

                        case mcomment:
                        {
                            if(s[i] == '*')
                            {
                                curr = asterisk;
                            }
                            else if(s[i] == '@' && (s[i - 1] == ' ' || (s[i - 1] == '/' && s[i - 2] == '/')))
                            {
                                curr = multitag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }

                        case asterisk:
                        {
                            if(s[i] == '*')
                            {
                                curr = asterisk;
                            }
                            else if(s[i] == '/')
                            {
                                curr = code;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                        }

                        case multitag:
                        {
                            if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '*')
                            {
                                curr = tagasterisk;
                                fprintf(stdout, "\n");
                            }
                            else
                            {
                                fprintf(stdout, "%c", s[i]);
                            }
                        }

                        case tagasterisk:
                        {
                            if(s[i] == '*')
                            {
                                fprintf(stdout, "*");
                                curr = tagasterisk;
                            }
                            else if(s[i] == '/')
                            {
                                fprintf(stdout, "\n");
                                curr = code;
                            }
                            else if(s[i] == ' ')
                            {
                                fprintf(stdout, "*\n");
                                curr = mcomment;
                            }
                            else
                            {
                                curr = multitag;
                                fprintf(stdout, "*%c", s[i]);
                            }
                        }

                        case beginmc:
                        {
                            curr = beginmc;
                        }

                        case beginlc:
                        {
                            curr = beginlc;
                        }

                        case asterisk2:
                        {
                            curr = asterisk2;
                        }
                    }
                }
            }
        }
        else if(dashl || (dasha == false && dashl == false))
        {
            enum state curr = code;
            while (fgets(s, 1000, stdin) != NULL)
            {
            
                    if (curr == code)
                    {
                        curr = code;
                    }

                    if (curr == slash)
                    {
                        curr = slash;
                    }

                    if (curr == string)
                    {
                        curr = string;
                    }

                    if (curr == character)
                    {
                        curr = character;
                    }

                    if (curr == bracket)
                    {
                        curr = bracket;
                    }

                    if (curr == beginlc)
                    {
                        curr = code;
                    }

                    if (curr == singletag)
                    {
                        fprintf(stdout, "\n");
                        curr = code;
                    }

                    if (curr == beginmc)
                    {
                        curr = mcomment;
                    }

                    if (curr == asterisk2)
                    {
                        curr = mcomment;
                    }

                    if (curr == lcomment)
                    {
                        curr = lcomment;
                    }

                    if (curr == mcomment)
                    {
                        curr = beginmc;
                    }

                    if (curr == asterisk)
                    {
                        curr = mcomment;
                    }

                    if (curr == multitag)
                    {
                        fprintf(stdout, "\n");
                        curr = mcomment;
                    }

                    if (curr == tagasterisk)
                    {
                        fprintf(stdout, "\n");
                        curr = mcomment;
                    }
                }

                for (int i = 0; s[i] != '\0'; i++)
                {
                    switch (curr)
                    {
                        case code:
                        {
                            if(s[i] == '/')
                            {
                                curr = slash;
                            }
                            else if(s[i] == '"')
                            {
                                curr = string;
                            }
                            else if(s[i] == '\'')
                            {
                                curr = character;
                            }
                            else if(s[i] == '{')
                            {
                                curr = bracket;
                            }
                            break;
                        }

                        case lcomment:
                        {
                            curr = lcomment;
                        }

                        case slash:
                        {
                            if(s[i] == '/')
                            {
                                curr = beginlc;
                            }
                            else if(s[i] == '*')
                            {
                                curr = beginmc;
                            }
                            else
                            {
                                curr = code;
                            }
                            break;
                        }

                        case string:
                        {
                            if(s[i] == '"')
                            {
                                curr = code;
                            }
                            break;
                        }

                        case character:
                        {
                            if(s[i] == '\'')
                            {
                                curr = code;
                            }
                            break;
                        }

                        case bracket:
                        {
                            if(s[i] == '}')
                            {
                                curr = code;
                            }
                            break;
                        }
                    
                        case beginlc:
                        {
                            if(s[i] == '@')
                            {
                                fprintf(stdout, "%c", s[i]);
                                curr = singletag;
                            }
                            else if(s[i] == '*' || isspace(s[i]) != 0)
                            {
                                curr = beginlc;
                            }
                            else
                            {
                                curr = code;
                            }
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == ' ')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else
                            {
                                fprintf(stdout, "%c", s[i]);
                                curr = singletag;
                            }
                            break;
                        }

                        case beginmc:
                        {
                            if(s[i] == '@')
                            {
                                curr = multitag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            else if(s[i] == ' ')
                            {
                                curr = beginlc;
                            }
                            else if (s[i] == '*')
                            {
                                curr = asterisk2;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                            break;
                        }

                        case asterisk2:
                        {
                            if(s[i] == '*')
                            {
                                curr = asterisk2;
                            }
                            else if(s[i] == '/')
                            {
                                curr = code;
                            }
                            else if(s[i] == ' ')
                            {
                                curr = beginmc;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                        }

                        case mcomment:
                        {
                            if(s[i] == '*')
                            {
                                curr = asterisk;
                            }
                            break;
                        }

                        case asterisk:
                        {
                            if(s[i] == '*')
                            {
                                curr = asterisk;
                            }
                            else if(s[i] == '/')
                            {
                                curr = code;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                        }

                        case multitag:
                        {
                            if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '*')
                            {
                                curr = tagasterisk;
                            }
                            else
                            {
                                fprintf(stdout, "%c", s[i]);
                            }
                        }

                        case tagasterisk:
                        {
                            if(s[i] == '*')
                            {
                                fprintf(stdout, "*");
                                curr = tagasterisk;
                            }
                            else if(s[i] == '/')
                            {
                                fprintf(stdout, "\n");
                                curr = code;
                            }
                            else if(s[i] == ' ')
                            {
                                fprintf(stdout, "*");
                                curr = mcomment;
                            }
                            else
                            {
                                fprintf(stdout, "*%c", s[i]);
                                curr = multitag;
                            }
                        }
                    }
                }
    }
}