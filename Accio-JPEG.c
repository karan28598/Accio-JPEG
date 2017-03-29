   /*Copyright (c) 2017 Karan Agrawal

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.*/

#include <stdio.h>
#include <stdint.h>


int main(int argc, char *argv[])
{
if (argc != 2)
{

    fprintf(stderr, "Usage: ./recover file\n");
    return 1;
}

//declaring pointer infile and giving the address of argv[1];
char *infile = argv[1];

//Opening file
FILE *raw_data; 
raw_data = fopen(infile, "r");

//Checking for NULL error
if(raw_data == NULL)
{
    fprintf(stderr, "Could not open file.\n");
    return 2;
}

uint8_t buffer[512];                //Delcaring unsigned int variable type. Array of 512 bytes.

int counter = 0;                    //Declaring counter for counting jpegs files

FILE *outfile;                    //Setting pointer named outfile for printing here

char filename[8];                 //declaring 'filename' variable for storing the file's name


//While we can reads blocks of memory of 512 bytes from raw_data (aka the address from the infile) into buffer:
while (fread(buffer, 512, 1, raw_data))
{
    //Condition for tracking the first bytes that form a JPEG file
    if(buffer[0] == 0xff && 
       buffer[1] == 0xd8 && 
       buffer[2] == 0xff &&
      (buffer[3] & 0xf0) == 0xe0)
      {

          if(counter != 0)                              
          {           
              fclose(outfile);                      //If this is not the first JPG opened, close previous file
          }

          sprintf(filename, "%03d.jpg", counter);  //print stream to 'filename' the value of 'counter' in 3 digits  
          outfile = fopen(filename, "w");           //Open file named outfile in write mode
          counter++;                                //Add 1 to counter

      } 
      if(counter != 0)                          //Don't start writing on a file until the first jpeg is found
      {
        fwrite(buffer, sizeof(buffer), 1, outfile);          /* - Write function that takes buffer data
                                                                    (aka the array of elements to be written) , 
                                                                - Write a block of 512 bytes of elements 
                                                                    (aka the size of buffer), 
                                                                - 1 block of 512 bytes at a time, 
                                                                - And it writes it to the output, aka 'outfile' */
      }                                                         

}
fclose(outfile);            //Remember to close the last file once we get out of the while-loop
return 0;
}