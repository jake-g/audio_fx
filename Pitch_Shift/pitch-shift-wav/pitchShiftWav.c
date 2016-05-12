#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <math.h>

#define NAME_SIZE 100
/*

Shifts pitch by given factor. Input and output are .wav files


On Linux:
sudo apt-get install libsndfile1
sudo apt-get install libsndfile1-dev

Build:
run makefile

Notes: libsndfile
http://www.mega-nerd.com/libsndfile/api.html
http://www.labbookpages.co.uk/audio/wavFiles.html
*/

void printBuffer(float *arr, long size);
void PitchShift(float pitchShift, long numSamps, long fftFrameSize, long osamp, float sampleRate, float *indata, float *outdata);
void handleInput(int argc, char ** argv, long * semitones, char inFileName[], char fileExtension[]);

// command line arg format: ./pitchShift semitones inputFilename_with_extension
int main(int argc, char * argv[])
{

    // int num_channels;
    long buffer_size;
    float *buf, sr;
    int num, f, c;
    SNDFILE *input, *output;
    SF_INFO info;

    /* Set Pitch Shift Factor*/
    long semitones;							// shift up by 3 semitones
  	float pitchShift;	// convert semitones to factor

    // file input/output handle
  	char inFileName[NAME_SIZE];      // file name without file extension
    char fileExtension[NAME_SIZE];   // file extension of both in/output
  	char outFileName[NAME_SIZE];     // format: "inputName-out.extension"
    char srcPath[NAME_SIZE]; // assuming input is in sound_files directory
    handleInput(argc, argv, &semitones, inFileName, fileExtension);
    sprintf(outFileName, "%s-out.%s", inFileName, fileExtension);
    sprintf(srcPath, "sound_files/%s.%s", inFileName, fileExtension);
    pitchShift = pow(2., semitones/12.);

    /* Open the WAV file. */
    info.format = 0;
    input = sf_open(srcPath, SFM_READ,&info);
    if (input == NULL)
    {
        printf("Failed to open the file.\n");
        exit(-1);
    }

    /* Print some of the info */
    f = info.frames;
    sr = info.samplerate;
    c = info.channels;
    buffer_size = f*c;
    printf("frames=%d\n",f);
    printf("samplerate=%lf\n",sr);
    printf("channels=%d\n",c);
    printf("buffer_size=%ld\n",buffer_size);

    /* Allocate space for the data to be read, then read it. */
    buf = (float *) malloc(buffer_size*sizeof(float));
    num = sf_readf_float(input, buf, buffer_size);
    sf_close(input);
    printf("Read %d intput items\n", num);

//    printBuffer(buf, buffer_size);

    /* Call PitchShift() */
    PitchShift(pitchShift, buffer_size, 2048, 4, sr, buf, buf);

     /*  Open sound file for writing */
  	output = sf_open(outFileName, SFM_WRITE, &info);
    num = sf_writef_float(output, buf, buffer_size);
    sf_close(output);
    printf("Read %d output items\n",num);

    return 0;
}


/* Print contents of data (float array 1 row per channel) */
void printBuffer(float *arr, long size)
{
    for (int i = 0; i < size; i++) {
        printf("%f ", arr[i]);
    }
}

void handleInput(int argc, char ** argv, long * semitones, char inFileName[], char fileExtension[]) {
  if (argc != 3) {
    printf("Needs 2 arguments\n");
    exit(0);
  }

  *semitones = atoi(argv[1]);

  char del[] = ".";
  char *ptr;
  ptr = strtok(argv[2], del);
  if (ptr != NULL) {
    strcpy(inFileName, ptr);
    strcpy(fileExtension, strtok(NULL, del));
  }
}
