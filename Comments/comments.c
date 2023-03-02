#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define Error (stderr)

enum state {code, slash, string, character, bracket, beginlc, beginmc, lcomment, mcomment, singletag, multitag, asterisk, asterisk2};

int stdhelper(char s[], int index);

int main(int argc, char *argv[])
{
    bool input = false;
    int indexi;
    bool output = false;
    int indexo;
    bool dasha = false;
    bool dashl = false;

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
        }
        else if(strcmp(argv[i], "-l") == 0)
        {
            dashl = true;
        }
    }

    char s[10000];

    if (!input && !output)
    {

        if(dasha)
        {
            enum state curr = code;
            int countb = 0;
            int countbmc = 0;
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
                        curr = code;
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
                                countb = 1;
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
                            if(countbmc == 1)
                            {
                                if(s[i] != '*' && s[i + 1] != '/')
                                {
                                    s[i] = 'b';
                                }
                                else if(s[i] == '*' && s[i + 1] == '/')
                                {
                                    countbmc = 0;
                                }
                                
                            }
                            else if (countbmc == 0)
                            {
                                if(s[i] == '{')
                                {
                                    countb++;
                                }
                                else if(s[i] == '}')
                                {
                                    countb--;
                                    if(countb == 0)
                                    {
                                        curr = code;
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '/')
                                {
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        s[i+j] = 'b';
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '*')
                                {
                                    countbmc = 1;
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        if (s[i + j] != '*' && s[i + j + 1] != '/')
                                        {
                                            s[i + j] = 'b';
                                        }
                                        else if(s[i+j] == '*' && s[i + j + 1] == '/')
                                        {
                                            countbmc = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
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
            int countbmc = 0;
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
                            if(countbmc == 1)
                            {
                                if(s[i] != '*' && s[i + 1] != '/')
                                {
                                    s[i] = 'b';
                                }
                                else if(s[i] == '*' && s[i + 1] == '/')
                                {
                                    countbmc = 0;
                                }
                                
                            }
                            else if (countbmc == 0)
                            {
                                if(s[i] == '{')
                                {
                                    countb++;
                                }
                                else if(s[i] == '}')
                                {
                                    countb--;
                                    if(countb == 0)
                                    {
                                        curr = code;
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '/')
                                {
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        s[i+j] = 'b';
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '*')
                                {
                                    countbmc = 1;
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        if (s[i + j] != '*' && s[i + j + 1] != '/')
                                        {
                                            s[i + j] = 'b';
                                        }
                                        else if(s[i+j] == '*' && s[i + j + 1] == '/')
                                        {
                                            countbmc = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
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

    if (input && !output)
    {
        FILE* in = fopen(argv[indexi + 1], "r");
        if(!in)
        {
            fprintf(stderr, "%s: couldn't open %s\n", argv[0], argv[indexi + 1]);
            return 1;
        }

        if(dasha)
        {
            enum state curr = code;
            int countb = 0;
            int countbmc = 0;
            while (fgets(s, 1000, in) != NULL)
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
                        curr = code;
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
                            if(countbmc == 1)
                            {
                                if(s[i] != '*' && s[i + 1] != '/')
                                {
                                    s[i] = 'b';
                                }
                                else if(s[i] == '*' && s[i + 1] == '/')
                                {
                                    countbmc = 0;
                                }
                                
                            }
                            else if (countbmc == 0)
                            {
                                if(s[i] == '{')
                                {
                                    countb++;
                                }
                                else if(s[i] == '}')
                                {
                                    countb--;
                                    if(countb == 0)
                                    {
                                        curr = code;
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '/')
                                {
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        s[i+j] = 'b';
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '*')
                                {
                                    countbmc = 1;
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        if (s[i + j] != '*' && s[i + j + 1] != '/')
                                        {
                                            s[i + j] = 'b';
                                        }
                                        else if(s[i+j] == '*' && s[i + j + 1] == '/')
                                        {
                                            countbmc = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
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
            int countbmc = 0;
            while (fgets(s, 1000, in) != NULL)
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
                            if(countbmc == 1)
                            {
                                if(s[i] != '*' && s[i + 1] != '/')
                                {
                                    s[i] = 'b';
                                }
                                else if(s[i] == '*' && s[i + 1] == '/')
                                {
                                    countbmc = 0;
                                }
                                
                            }
                            else if (countbmc == 0)
                            {
                                if(s[i] == '{')
                                {
                                    countb++;
                                }
                                else if(s[i] == '}')
                                {
                                    countb--;
                                    if(countb == 0)
                                    {
                                        curr = code;
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '/')
                                {
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        s[i+j] = 'b';
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '*')
                                {
                                    countbmc = 1;
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        if (s[i + j] != '*' && s[i + j + 1] != '/')
                                        {
                                            s[i + j] = 'b';
                                        }
                                        else if(s[i+j] == '*' && s[i + j + 1] == '/')
                                        {
                                            countbmc = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
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
        fclose(in);
    }
    if (!input && output)
    {
        FILE* out = fopen(argv[indexo + 1], "w");
        if(!out)
        {
            fprintf(stderr, "%s: couldn't open %s\n", argv[0], argv[indexo + 1]);
            return 1;
        }

        if(dasha)
        {
            enum state curr = code;
            int countb = 0;
            int countbmc = 0;
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
                        curr = code;
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
                        fprintf(out, "\n");
                        curr = code;
                    }

                    if (curr == multitag)
                    {
                        if (s[0] == ' ' || s[0] == '@')
                        {
                            fprintf(out, "\n");
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
                            if(countbmc == 1)
                            {
                                if(s[i] != '*' && s[i + 1] != '/')
                                {
                                    s[i] = 'b';
                                }
                                else if(s[i] == '*' && s[i + 1] == '/')
                                {
                                    countbmc = 0;
                                }
                                
                            }
                            else if (countbmc == 0)
                            {
                                if(s[i] == '{')
                                {
                                    countb++;
                                }
                                else if(s[i] == '}')
                                {
                                    countb--;
                                    if(countb == 0)
                                    {
                                        curr = code;
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '/')
                                {
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        s[i+j] = 'b';
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '*')
                                {
                                    countbmc = 1;
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        if (s[i + j] != '*' && s[i + j + 1] != '/')
                                        {
                                            s[i + j] = 'b';
                                        }
                                        else if(s[i+j] == '*' && s[i + j + 1] == '/')
                                        {
                                            countbmc = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }

                        case beginlc:
                        {
                            if(s[i] == '@' && (s[i - 1] == ' ' || s[i - 1] == '*' || (s[i - 1] == '/' && s[i - 2] == '/')))
                            {
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "%c", s[i]);
                            } 
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == ' ')
                            {
                                curr = lcomment;
                                fprintf(out, "\n");
                            }
                            else if(s[i + 1] == '\0')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = singletag;
                            }
                            else
                            {
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\' || s[i] == '\n')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                fprintf(out, "%c", s[i]);
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
            int countbmc = 0;
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
                    fprintf(out, "\n");
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
                        fprintf(out, "\n");
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
                            if(countbmc == 1)
                            {
                                if(s[i] != '*' && s[i + 1] != '/')
                                {
                                    s[i] = 'b';
                                }
                                else if(s[i] == '*' && s[i + 1] == '/')
                                {
                                    countbmc = 0;
                                }
                                
                            }
                            else if (countbmc == 0)
                            {
                                if(s[i] == '{')
                                {
                                    countb++;
                                }
                                else if(s[i] == '}')
                                {
                                    countb--;
                                    if(countb == 0)
                                    {
                                        curr = code;
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '/')
                                {
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        s[i+j] = 'b';
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '*')
                                {
                                    countbmc = 1;
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        if (s[i + j] != '*' && s[i + j + 1] != '/')
                                        {
                                            s[i + j] = 'b';
                                        }
                                        else if(s[i+j] == '*' && s[i + j + 1] == '/')
                                        {
                                            countbmc = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    
                        case beginlc:
                        {
                            if(s[i] == '@' && (s[i - 1] == ' ' || s[i - 1] == '*' || (s[i - 1] == '/' && s[i - 2] == '/')))
                            {
                                fprintf(out, "%c", s[i]);
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

                        case singletag:
                        {
                            if(s[i] == ' ' || s[i + 1] == '\0')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = singletag;
                            }
                            else
                            {
                                fprintf(out, "%c", s[i]);
                                curr = singletag;
                            }
                            break;
                        }

                        case beginmc:
                        {
                            if(s[i] == '@')
                            {
                                curr = multitag;
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\' || s[i] == '\n')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                curr = multitag;
                                fprintf(out, "%c", s[i]);
                            }
                            break;
                        }

                        
                    }
                }
            }
        }
        fclose(out);
    }
    if (input && output)
    {
        FILE* in = fopen(argv[indexi + 1], "r");
        if(!in)
        {
            fprintf(stderr, "%s: couldn't open %s\n", argv[0], argv[indexi + 1]);
            return 1;
        }
        FILE* out = fopen(argv[indexo + 1], "w");
        if(!out)
        {
            fprintf(stderr, "%s: couldn't open %s\n", argv[0], argv[indexo + 1]);
            return 1;
        }

        if(dasha)
        {
            enum state curr = code;
            int countb = 0;
            int countbmc = 0;
            while (fgets(s, 1000, in) != NULL)
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
                        curr = code;
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
                        fprintf(out, "\n");
                        curr = code;
                    }

                    if (curr == multitag)
                    {
                        if (s[0] == ' ' || s[0] == '@')
                        {
                            fprintf(out, "\n");
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
                            if(countbmc == 1)
                            {
                                if(s[i] != '*' && s[i + 1] != '/')
                                {
                                    s[i] = 'b';
                                }
                                else if(s[i] == '*' && s[i + 1] == '/')
                                {
                                    countbmc = 0;
                                }
                                
                            }
                            else if (countbmc == 0)
                            {
                                if(s[i] == '{')
                                {
                                    countb++;
                                }
                                else if(s[i] == '}')
                                {
                                    countb--;
                                    if(countb == 0)
                                    {
                                        curr = code;
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '/')
                                {
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        s[i+j] = 'b';
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '*')
                                {
                                    countbmc = 1;
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        if (s[i + j] != '*' && s[i + j + 1] != '/')
                                        {
                                            s[i + j] = 'b';
                                        }
                                        else if(s[i+j] == '*' && s[i + j + 1] == '/')
                                        {
                                            countbmc = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }

                        case beginlc:
                        {
                            if(s[i] == '@' && (s[i - 1] == ' ' || s[i - 1] == '*' || (s[i - 1] == '/' && s[i - 2] == '/')))
                            {
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "%c", s[i]);
                            } 
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == ' ')
                            {
                                curr = lcomment;
                                fprintf(out, "\n");
                            }
                            else if(s[i + 1] == '\0')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = singletag;
                            }
                            else
                            {
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\' || s[i] == '\n')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                fprintf(out, "%c", s[i]);
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
            int countbmc = 0;
            while (fgets(s, 1000, in) != NULL)
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
                    fprintf(out, "\n");
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
                        fprintf(out, "\n");
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
                            if(countbmc == 1)
                            {
                                if(s[i] != '*' && s[i + 1] != '/')
                                {
                                    s[i] = 'b';
                                }
                                else if(s[i] == '*' && s[i + 1] == '/')
                                {
                                    countbmc = 0;
                                }
                                
                            }
                            else if (countbmc == 0)
                            {
                                if(s[i] == '{')
                                {
                                    countb++;
                                }
                                else if(s[i] == '}')
                                {
                                    countb--;
                                    if(countb == 0)
                                    {
                                        curr = code;
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '/')
                                {
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        s[i+j] = 'b';
                                    }
                                }
                                else if(s[i] == '/' && s[i + 1] == '*')
                                {
                                    countbmc = 1;
                                    for(int j = 2; s[i + j + 1] != '\0'; j++)
                                    {
                                        if (s[i + j] != '*' && s[i + j + 1] != '/')
                                        {
                                            s[i + j] = 'b';
                                        }
                                        else if(s[i+j] == '*' && s[i + j + 1] == '/')
                                        {
                                            countbmc = 0;
                                            break;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                    
                        case beginlc:
                        {
                            if(s[i] == '@' && (s[i - 1] == ' ' || s[i - 1] == '*' || (s[i - 1] == '/' && s[i - 2] == '/')))
                            {
                                fprintf(out, "%c", s[i]);
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

                        case singletag:
                        {
                            if(s[i] == ' ' || s[i + 1] == '\0')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = singletag;
                            }
                            else
                            {
                                fprintf(out, "%c", s[i]);
                                curr = singletag;
                            }
                            break;
                        }

                        case beginmc:
                        {
                            if(s[i] == '@')
                            {
                                curr = multitag;
                                fprintf(out, "%c", s[i]);
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
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\' || s[i] == '\n')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                curr = multitag;
                                fprintf(out, "%c", s[i]);
                            }
                            break;
                        }

                        
                    }
                }
            }
        }
        fclose(in);
        fclose(out);
    }

}