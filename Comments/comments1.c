#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define Error (stderr)

enum state {code, slash, string, character, bracket, beginlc, beginmc, lcomment, mcomment, singletag, multitag};

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

    if(input && !output)
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
            while (fgets(s, 1000, in) != NULL)
            {
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
                            {
                                curr = singletag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            else if(s[i] == '*' || s[i] == ' ')
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@' && s[i - 1] == ' ')
                            {
                                curr = singletag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == ' ')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = singletag;
                                fprintf(stdout, "\n");
                            }
                            else
                            {
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }

                        case beginmc:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
                            {
                                curr = multitag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            else if(s[i] == '*' || s[i] == ' ')
                            {
                                curr = beginlc;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                            
                            break;
                        }

                        case mcomment:
                        {
                            if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@' && s[i - 1] == ' ')
                            {
                                curr = multitag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }

                        case multitag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = multitag;
                            }
                            else if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }
                    }

                }
            }
        }
        else if (dashl || (!dashl && !dasha))
        {
            enum state curr = code;

            while (fgets(s, 1000, in) != NULL)
            {
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
                            {
                                printf("%c", s[i]);
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                                printf("\n");
                            }
                            else if(s[i] == ' ')
                            {
                                curr = code;
                                printf("\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = singletag;
                            }
                            else
                            {
                                printf("%c", s[i]);
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
                            else if(s[i] == '*' || s[i] == ' ')
                            {
                                curr = beginlc;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                            
                            break;
                        }

                        case mcomment:
                        {
                            if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                            }
                            else if (s[i] == '\0')
                            {
                                curr = beginmc;
                            }

                            break;
                        }

                        case multitag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = multitag;
                            }
                            else if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = multitag;
                            }
                            else
                            {
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
    else if (input && output)
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
            while (fgets(s, 1000, in) != NULL)
            {
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
                            {
                                curr = singletag;
                                fprintf(out, "%c", s[i]);
                            }
                            else if(s[i] == '*' || s[i] == ' ')
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@' && s[i - 1] == ' ')
                            {
                                curr = singletag;
                                fprintf(out, "%c", s[i]);
                            }
                            
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == ' ')
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

                        case beginmc:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
                            {
                                curr = multitag;
                                fprintf(out, "%c", s[i]);
                            }
                            else if(s[i] == '*' || s[i] == ' ')
                            {
                                curr = beginlc;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                            
                            break;
                        }

                        case mcomment:
                        {
                            if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@' && s[i - 1] == ' ')
                            {
                                curr = multitag;
                                fprintf(out, "%c", s[i]);
                            }
                            break;
                        }

                        case multitag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = multitag;
                            }
                            else if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                fprintf(out, "%c", s[i]);
                            }
                            break;
                        }
                    }

                }
            }
        }
        else if (dashl || (!dashl && !dasha))
        {
            enum state curr = code;

            while (fgets(s, 1000, in) != NULL)
            {
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == ' ')
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
                            else if(s[i] == '*' || s[i] == ' ')
                            {
                                curr = beginlc;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                            
                            break;
                        }

                        case mcomment:
                        {
                            if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                            }
                            else if (s[i] == '\0')
                            {
                                curr = beginmc;
                            }

                            break;
                        }

                        case multitag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = multitag;
                            }
                            else if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(out, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = multitag;
                            }
                            else
                            {
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
    else if (!input && !output)
    {
        if(dasha)
        {
            enum state curr = code;
            while (fgets(s, 1000, stdin) != NULL)
            {
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
                            {
                                curr = singletag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            else if(s[i] == '*' || s[i] == ' ')
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@' && s[i - 1] == ' ')
                            {
                                curr = singletag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == ' ')
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

                        case beginmc:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
                            {
                                curr = multitag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            else if(s[i] == '*' || s[i] == ' ')
                            {
                                curr = beginlc;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                            
                            break;
                        }

                        case mcomment:
                        {
                            if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@' && s[i - 1] == ' ')
                            {
                                curr = multitag;
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }

                        case multitag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = multitag;
                            }
                            else if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\')
                            {
                                curr = multitag;
                            }
                            else
                            {
                                fprintf(stdout, "%c", s[i]);
                            }
                            break;
                        }
                    }

                }
            }
        }
        else if (dashl || (!dashl && !dasha))
        {
            enum state curr = code;

            while (fgets(s, 1000, stdin) != NULL)
            {
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            else if(s[i] == '@')
                            {
                                printf("%c", s[i]);
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
                            if(s[i] == '\0')
                            {
                                curr = code;
                            }
                            
                            break;
                        }

                        case singletag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = code;
                                printf("\n");
                            }
                            else if(s[i] == ' ')
                            {
                                curr = code;
                                printf("\n");
                            }
                            else if(s[i] == '\\' || s[i] == '"' || s[i] == '\'')
                            {
                                curr = singletag;
                                fprintf(stdout, "\n");
                            }
                            else
                            {
                                printf("%c", s[i]);
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
                            else if(s[i] == '*' || s[i] == ' ')
                            {
                                curr = beginlc;
                            }
                            else
                            {
                                curr = mcomment;
                            }
                            
                            break;
                        }

                        case mcomment:
                        {
                            if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                            }
                            else if (s[i] == '\0')
                            {
                                curr = beginmc;
                            }

                            break;
                        }

                        case multitag:
                        {
                            if(s[i] == '\0')
                            {
                                curr = multitag;
                            }
                            else if(s[i] == ' ')
                            {
                                curr = mcomment;
                                fprintf(stdout, "\n");
                            }
                            else if(s[i] == '\\' || s[i] == '"' || s[i] == '\'')
                            {
                                curr = multitag;
                            }
                            else if(s[i] == '*' && s[i + 1] == '/')
                            {
                                curr = code;
                                fprintf(stdout, "\n");
                            }
                            else
                            {
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