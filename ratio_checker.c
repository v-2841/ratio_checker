#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 512

int findAreaColumn(const char *line)
{
    const char *delim = "\t";
    char *token = strtok((char *)line, delim);
    int column = 0;
    while (token != NULL)
    {
        if (strcmp(token, "Area") == 0)
        {
            return column;
        }
        token = strtok(NULL, delim);
        column++;
    }
    return -1;
}

double extractArea(const char *line, int areaColumn)
{
    const char *delim = "\t";
    char *token = strtok((char *)line, delim);
    int column = 0;
    double area = 0.0;
    while (token != NULL)
    {
        if (column == areaColumn)
        {
            area = atof(token);
            break;
        }
        token = strtok(NULL, delim);
        column++;
    }
    return area;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s filename.txt\n", argv[0]);
        printf("Press Enter to exit...\n");
        getchar();
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Error opening file\n");
        printf("Press Enter to exit...\n");
        getchar();
        return 1;
    }
    char line[MAX_LINE_LENGTH];
    int areaColumn = -1;
    double c120 = 0.0, c140 = 0.0, c160 = 0.0, c180 = 0.0, c181 = 0.0, c182 = 0.0;
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        areaColumn = findAreaColumn(line);
        if (areaColumn != -1)
        {
            break;
        }
    }
    if (areaColumn == -1)
    {
        printf("\"Area\" not found in the file.\n");
        fclose(file);
        printf("Press Enter to exit...\n");
        getchar();
        return 1;
    }
    while (fgets(line, MAX_LINE_LENGTH, file))
    {
        if (strstr(line, "C12:0"))
            c120 = extractArea(line, areaColumn);
        else if (strstr(line, "C14:0"))
            c140 = extractArea(line, areaColumn);
        else if (strstr(line, "C16:0"))
            c160 = extractArea(line, areaColumn);
        else if (strstr(line, "C18:0"))
            c180 = extractArea(line, areaColumn);
        else if (strstr(line, "C18:1 trans"))
            c181 = extractArea(line, areaColumn);
        else if (strstr(line, "C18:1 cis"))
            c181 += extractArea(line, areaColumn);
        else if (strstr(line, "C18:2 trans"))
            c182 = extractArea(line, areaColumn);
        else if (strstr(line, "C18:2 cis"))
        {
            c182 += extractArea(line, areaColumn);
            break;
        }
    }
    fclose(file);
    if (c120 == 0 || c140 == 0 || c160 == 0 || c180 == 0 || c181 == 0 || c182 == 0)
    {
        printf("\"Something is rotten in the state of Denmark\". Some acids are none.'n");
        printf("Press Enter to exit...\n");
        getchar();
        return 1;
    }
    printf("%s\n\n", argv[1]);
    double ratio = c160 / c120;
    printf("C16:0 : C12:0 = %f (5.8 - 14.5) - %s\n",
           ratio, (ratio >= 5.8 && ratio <= 14.5) ? "Yes" : "No");
    ratio = c180 / c120;
    printf("C18:0 : C12:0 = %f (1.9 - 5.9) - %s\n",
           ratio, (ratio >= 1.9 && ratio <= 5.9) ? "Yes" : "No");
    ratio = c181 / c140;
    printf("C18:1 : C14:0 = %f (1.6 - 3.6) - %s\n",
           ratio, (ratio >= 1.6 && ratio <= 3.6) ? "Yes" : "No");
    ratio = c182 / c140;
    printf("C18:2 : C14:0 = %f (0.1 - 0.5) - %s\n",
           ratio, (ratio >= 0.1 && ratio <= 0.5) ? "Yes" : "No");
    ratio = (c181 + c182) / (c120 + c140 + c160 + c180);
    printf("C18:1 + C18:2 : C12:0 + C14:0 + C16:0 + C18:0 = %f (0.4 - 0.7) - %s\n\n",
           ratio, (ratio >= 0.4 && ratio <= 0.7) ? "Yes" : "No");
    printf("Press Enter to exit...\n");
    getchar();
    return 0;
}
