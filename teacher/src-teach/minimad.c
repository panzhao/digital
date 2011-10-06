#include<stdio.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include<sys/stat.h>  
#include<sys/mman.h>  
#include<sys/soundcard.h>  
#include<sys/ioctl.h>  
#include<sys/fcntl.h>  
#include<sys/types.h>  
#include"mad.h"
struct buffer {  
	unsigned char const *start;  
	unsigned long length;  
};  
static int fds;         /*声音设备的描述符 */ 
static int decode(unsigned char const *, unsigned long);  
int music(void)  
{  
	struct stat stat;  
	void *fdm;  
	int fd;  

	fd = open("3.mp3", O_RDONLY);  //必须是只读打开
	if ((fds = open("/dev/dsp", O_WRONLY)) < 0) 
    {  
		printf("can not open dev/dsp!\n");  
		return 1;  
	}  
	ioctl(fds, SNDCTL_DSP_SYNC, 0);  
	if (fstat(fd, &stat) == -1 || stat.st_size == 0)  
    {
		return 2;  
    }

	fdm = mmap(0, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);  
	if (fdm == MAP_FAILED)  
	{
		printf("music  mmap failed\n");
		return 3;  
	}
	decode(fdm, stat.st_size);  
	if (munmap(fdm, stat.st_size) == -1)  
		return 4;  
	ioctl(fds, SNDCTL_DSP_RESET, 0);  
	close(fds);  

	return 0;  
}  
static enum mad_flow input(void *data, struct mad_stream *stream)  
{  
	struct buffer *buffer = data;  
	if (!buffer->length) 
    {
		return MAD_FLOW_STOP;  
    }

	mad_stream_buffer(stream, buffer->start, buffer->length);  
	buffer->length = 0;  

	return MAD_FLOW_CONTINUE;  
}  
/*这一段是处理采样后的pcm音频 */ 
static inline signed int scale(mad_fixed_t sample)  
{  
	sample += (1L << (MAD_F_FRACBITS - 16));  
	if (sample >= MAD_F_ONE)  
		sample = MAD_F_ONE - 1;  
	else if (sample < -MAD_F_ONE)  
		sample = -MAD_F_ONE;  

	return sample >> (MAD_F_FRACBITS + 1 - 16);  
}  
static enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm)  
{  
	unsigned int nchannels, nsamples, n;  
	mad_fixed_t const *left_ch, *right_ch;  
	unsigned char Output[6912], *Ptr;  
	int fmt, wrote, speed;  

	nchannels = pcm->channels;  
	n = nsamples = pcm->length;  
	left_ch = pcm->samples[0];  
	right_ch = pcm->samples[1];  

	fmt = AFMT_S16_LE;  
	speed = pcm->samplerate * 2; /*播放速度是采样率的两倍 */ 
	ioctl(fds, SNDCTL_DSP_SPEED, &(speed));  
	ioctl(fds, SNDCTL_DSP_SETFMT, &fmt);  
	ioctl(fds, SNDCTL_DSP_CHANNELS, &(pcm->channels));  
	Ptr = Output;  
	while (nsamples--) 
    {  
		signed int sample;  
		sample = scale(*left_ch++);  
		*(Ptr++) = sample >> 0;  
		*(Ptr++) = sample >> 8;  
		if (nchannels == 2) 
        {  
			sample = scale(*right_ch++);  
			*(Ptr++) = sample >> 0;  
			*(Ptr++) = sample >> 8;  
		}  
	}  
	n *= 4;         /*数据长度为pcm音频采样的4倍 */ 
	Ptr = Output;  
	while (n) 
    {  
		wrote = write(fds, Ptr, n);  
		Ptr += wrote;  
		n -= wrote;  
	}  
	Ptr = Output;  

	return MAD_FLOW_CONTINUE;  
}  

static enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame)  
{  
	return MAD_FLOW_CONTINUE;  
}  

static int decode(unsigned char const *start, unsigned long length)  
{  
	struct buffer buffer;  
	struct mad_decoder decoder;  
	int result;  

	buffer.start = start;  
	buffer.length = length;  
	mad_decoder_init(&decoder, &buffer, input, 0, 0, output, error, 0);  
	mad_decoder_options(&decoder, 0);  
	result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);  
	mad_decoder_finish(&decoder);  

	return result;  
} 
