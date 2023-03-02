#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define Error (stderr)

enum state {code, slash, string, character, bracket, blcomment, beginlc, beginmc, lcomment, mcomment, singletag, multitag, asterisk, asterisk2};

int stdhelper(char s[], int index);

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

    for(int i = 0; i < argc; i++)
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
        else if(strcmp(argv[i], "-a") == 0)
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
            enum state curr = code;
            int countb = 0;
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

                    if (curr == blcomment)
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
                        if (s[0] == ' ' || s[0] == '@')
                        {
                            fprintf(stdout, "\n");
                            curr = mcomment;
                        }
                        else
                        {
                            curr = multitag;
                        }
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
                        curr = code;
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
                                countb++;
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
                                curr = beginlc;
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
                            if(s[i] == '{')
                            {
                                countb++;
                            }
                            else if(s[i] == '/' && s[i + 1] == '/')
                            {
                                curr = blcomment;
                            }
                            else if(s[i] == '}')
                            {
                                countb--;
                                if(countb == 0)
                                {
                                    curr = code;
                                }
                            }
                            break;
                        }

                        case blcomment:
                        {
                            curr = blcomment;
                        }

                        case beginlc:
                        {
                            if(s[i] == '@' && (s[i - 1] == ' ' || s[i - 1] == '*' || (s[i - 1] == '/' && s[i - 2] == '/')))
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
                                curr = lcomment;
                            }
                            break;
                        }

                        case lcomment:
                        {
                            if(s[i] == '@' && (s[i - 1] == ' '))
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
                            else if(s[i + 1] == '\0')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = singletag;
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
                            else if(s[i] == '@' && (s[i - 1] == ' ' || (s[i - 1] == '*' && s[i - 2] == '/')))
                            {
                                fprintf(stdout, "%c", s[i]);
                                curr = multitag;
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
                            else if(s[i] == '@')
                            {
                                fprintf(stdout, "%c", s[i]);
                                curr = multitag;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                            break;
                        }

                        case multitag:
                        {
                            if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\' || s[i] == '\n')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }

                        case beginmc:
                        {
                            curr = beginmc;
                        }

                        case asterisk2:
                        {
                            curr = asterisk2;
                        }
                    }
                }
            }
        }
        else if(dashl || (!dasha && !dashl))
        {
            enum state curr = code;
            int countb = 0;
            while (fgets(s, 1000, stdin) != NULL)
            {
                if (curr == code)
                {
                    curr = code;
                }

                if (curr == slash)
                {
                    curr = code;
                }

                if (curr == string)
                {
                    curr = code;
                }

                if (curr == character)
                {
                    curr = code;
                }

                if (curr == bracket)
                {
                    curr = bracket;
                }

                if (curr == blcomment)
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
                    curr = code;
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
                    if (s[0] == ' ' || s[0] == '@')
                    {
                        fprintf(stdout, "\n");
                        curr = beginmc;
                    }
                    else
                    {
                        curr = multitag;
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
                                countb++;
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
                            if(s[i] == '{')
                            {
                                countb++;
                            }
                            else if(s[i] == '/' && s[i + 1] == '/')
                            {
                                curr = blcomment;
                            }
                            else if(s[i] == '}')
                            {
                                countb--;
                                if(countb == 0)
                                {
                                    curr = code;
                                }
                            }
                            break;
                        }

                        case blcomment:
                        {
                            if(s[i + 1] == '\0')
                            {
                                curr = bracket;
                            }
                            else
                            {
                                curr = blcomment;
                            }
                        }
                    
                        case beginlc:
                        {
                            if(s[i] == '@' && (s[i - 1] == ' ' || s[i - 1] == '*' || (s[i - 1] == '/' && s[i - 2] == '/')))
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
                            if(s[i] == ' ' || s[i + 1] == '\0')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = singletag;
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
                                curr = beginmc;
                            }
                            else if (s[i] == '*')
                            {
                                if(s[i + 1] == '/')
                                {
                                    curr = code;
                                }
                                else
                                {
                                    curr = beginmc;
                                }
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
                            break;
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
                            break;
                        }

                        case multitag:
                        {
                            if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\' || s[i] == '\n')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                curr = multitag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }

                        
                    }
                }
            }
        }
    }

}
