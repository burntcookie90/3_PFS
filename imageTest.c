/*
 * libexif example program to display the contents of a number of specific
 * EXIF and MakerNote tags. The tags selected are those that may aid in
 * identification of the photographer who took the image.
 *
 * Placed into the public domain by Dan Fandrich
 */

#include <stdio.h>
#include <string.h>
#include <libexif/exif-data.h>
#include <time.h>

//Hold date
char year[5];
char day[3];
char monthName[10];

/* Remove spaces on the right of the string */
static void trim_spaces(char *buf)
{
    char *s = buf-1;
    for (; *buf; ++buf) {
        if (*buf != ' ')
            s = buf;
    }
    *++s = 0; /* nul terminate the string on the first of the final spaces */
}

static int today(){
	//if tag does not exist, print todays date
          int monthNum;
	   struct tm *current;
           time_t timenow;
           time(&timenow);
           current = localtime(&timenow);
           monthNum = current->tm_mon+1;
           day[sprintf(day, "%d", current->tm_mday)]='\0';
           year[sprintf(year, "%d", current->tm_year+1900)]='\0';
	   return monthNum;
}

/* Show the tag name and contents if the tag exists */
static int show_tag(ExifData *d, ExifIfd ifd, ExifTag tag)
{
    //hold month number
    int monthNum = 1;

    /* See if this tag exists */
    ExifEntry *entry = exif_content_get_entry(d->ifd[ifd],tag);
    if (entry) {
        char buf[1024];

        // Get the contents of the tag in human-readable form 
        exif_entry_get_value(entry, buf, sizeof(buf));

        // Check if date is actually there
        trim_spaces(buf);
        if (*buf) {
	     if (tag==EXIF_TAG_DATE_TIME_ORIGINAL){
		//Hold date
		char month[3];		

		//Extract date
	   	memcpy(year,buf, 4);
	    	year[4] = '\0';
	    	memcpy(month, buf+5, 2);
	    	month[2] = '\0';
	    	monthNum = atoi(month);
	    	memcpy(day, buf+8,2);
	    	day[2] = '\0';
	    }
	}else{
            if (tag==EXIF_TAG_DATE_TIME_ORIGINAL){
	     	//if no date is actually there, print todays date
	   	monthNum =  today();
	    }
	}
        
    }else{
	if (tag==EXIF_TAG_DATE_TIME_ORIGINAL){
	   //if tag does not exist, print todays date
          monthNum = today();
	}
    }

    if (tag==EXIF_TAG_DATE_TIME_ORIGINAL){ 
	return monthNum;
    }else{
	return 0;
    }
}

void datePost(int monthNum){

	 //Put month in word form
            switch (monthNum){
                case 1:
                    monthName[sprintf(monthName, "January")] = '\0';
                    break;
                case 2:
                    monthName[sprintf(monthName, "February")] = '\0';
                    break;
                case 3:
                    monthName[sprintf(monthName, "March")] = '\0';
                    break;
                case 4:
                    monthName[sprintf(monthName, "April")] = '\0';
                    break;
                case 5:
                    monthName[sprintf(monthName, "May")] = '\0';
                    break;
                case 6:
                    monthName[sprintf(monthName, "June")] = '\0';
                    break;
                case 7:
                    monthName[sprintf(monthName, "July")] = '\0';
                    break;
                case 8:
                    monthName[sprintf(monthName, "August")] = '\0';
                    break;
                case 9:
                    monthName[sprintf(monthName, "September")] = '\0';
                    break;
                case 10:
                    monthName[sprintf(monthName, "October")] = '\0';
                    break;
                case 11:
                    monthName[sprintf(monthName, "November")] = '\0';
                    break;
                case 12:
                    monthName[sprintf(monthName, "December")] = '\0';
                    break;
                default:
                        break;
            }
            printf("%s %s %s\n", day, monthName, year);

}

int main(int argc, char **argv)
{
    ExifData *ed;
    ExifEntry *entry;

    //Check arguments
    if (argc < 2) {
        printf("Usage: %s image.jpg\n", argv[0]);
        printf("Displays tags potentially relating to ownership "
                "of the image.\n");
        return 1;
    }
/*
    //Check for existence of file
    if (FILE * file = fopen(argv[0], "r")){
        fclose(file);
    }else{
    	return 2;
    }
*/
    // Load an ExifData object from an EXIF file
    ed = exif_data_new_from_file(argv[1]);
    if (!ed) {
	//Get current date
	datePost(today());
    }else{
	//Get date
	datePost(show_tag(ed, EXIF_IFD_EXIF, EXIF_TAG_DATE_TIME_ORIGINAL));
    	
	// Free the EXIF data
    	exif_data_unref(ed);
    }
    
    return 0;
}

