#include <stdio.h>

#define OPCODE_NOP 0
#define OPCODE_STOP 1
#define OPCODE_PAUSE 2
#define OPCODE_BREAK 3
#define OPCODE_FLUSHQ 4
#define OPCODE_PDELAY 5
#define OPCODE_QPDELAY 6
#define OPCODE_PYLON 7
#define OPCODE_QPYLON 8
#define OPCODE_NOPYLON 9
#define OPCODE_QNOPYLON 10
#define OPCODE_LSAFEON 11
#define OPCODE_SETLAYER 16
#define OPCODE_SETGRP 18
#define OPCODE_FLMOV 32
#define OPCODE_PTCOMP 128
#define OPCODE_PTCOMP2 129
#define OPCODE_SETPOS 144
#define OPCODE_SETOPWR 145
#define OPCODE_WAITOPWR 146
#define OPCODE_MMOV 160
#define OPCODE_TIMEDAXISMOV 161
#define OPCODE_WAITAXISMOV 162
#define OPCODE_TIMEDAXISPOS 163
#define OPCODE_WAITAXISPOS 164
#define OPCODE_TIMEDPOS 165
#define OPCODE_WAITPOS 166
#define OPCODE_QLINEACQ 176
#define OPCODE_QPOSACQ 177
#define OPCODE_QTIMEACQ 178
#define OPCODE_MMOVLINE 224
#define OPCODE_EXTEND 255

FILE* filein;
unsigned char charin;

struct opcode_raw {
  unsigned char byte0;
  unsigned char byte1;
  unsigned char byte2;
  unsigned char byte3;
  unsigned char byte4;
  unsigned char byte5;
  unsigned char byte6;
  unsigned char byte7;
  unsigned char byte8;
  unsigned char byte9;
  unsigned char byte10;
  unsigned char byte11;
};

struct opcode_simple32 {
  unsigned int opcode_number : 8;
  unsigned int padding : 24;
  unsigned int na;
  unsigned int na2;
};

struct opcode_mmov {
  unsigned int opcode_number : 8;
  int x_coor : 16;
  unsigned int y_coor_lo : 8;
  unsigned int y_coor_hi : 8;
  unsigned int time_inc : 20;
  unsigned int padding : 4;
  unsigned int na;
};

struct opcode_axismov {
  unsigned int opcode_number : 8;
  unsigned int condition : 1;
  unsigned int axis : 2;
  unsigned int padding : 5;
  int adc_count : 16;
  unsigned int timeout : 8;
  unsigned int padding2 : 24;
  unsigned int na;
};

struct opcode_axispos {
  unsigned int opcode_number : 8;
  unsigned int axis : 2;
  unsigned int padding : 6;
  int adc_count : 16;
  unsigned int tolerance : 8;
  unsigned int timeout : 8;
  unsigned int padding2 : 16;
  unsigned int na;
};

struct opcode_qposacq {
  unsigned int opcode_number : 8;
  int x_adc_count : 16;
  unsigned int y_adc_count_lo : 8;
  unsigned int y_adc_count_hi : 8;
  unsigned int x_count_tol : 8;
  unsigned int y_count_tol : 8;
  unsigned int imul : 8;
  unsigned int na;
};

struct opcode_pdelay {
  unsigned int opcode_number : 8;
  unsigned int time_inc : 20;
  unsigned int padding : 4;
  unsigned int na;
  unsigned int na2;
};

struct opcode_arg_uint8 {
  unsigned int opcode_number : 8;
  unsigned int arg : 8;
  unsigned int padding : 16;
  unsigned int na;
  unsigned int na2;
};

struct opcode_arg_uint16 {
  unsigned int opcode_number : 8;
  unsigned int arg : 16;
  unsigned int padding : 8;
  unsigned int na;
  unsigned int na2;
};

struct opcode_arg_uint24 {
  unsigned int opcode_number : 8;
  unsigned int arg : 24;
  unsigned int na;
  unsigned int na2;
};

struct opcode_qlineacq {
  unsigned int opcode_number : 8;
  unsigned int condition : 1;
  unsigned int axis : 1;
  unsigned int padding : 6;
  int starting_count : 16;
  unsigned int imul : 8;
  unsigned int rep : 16;
  int count_inc : 8;
};

struct opcode_waitpos {
  unsigned int opcode_number : 8;
  int x_adc_count : 16;
  unsigned int y_adc_count_lo : 8;
  unsigned int y_adc_count_hi : 8;
  unsigned int x_count_tol : 8;
  unsigned int y_count_tol : 8;
  unsigned int timeout : 8;
  unsigned int na;
};

struct opcode_qtimeacq {
  unsigned int opcode_number : 8;
  unsigned int imul : 8;
  unsigned int rep_lo : 16;
  unsigned int rep_hi : 8;
  unsigned int time_inc : 20;
  unsigned int padding : 4;
  unsigned int na;
};

struct opcode_setopwr {
  unsigned int opcode_number : 8;
  unsigned int target_lo : 24;
  unsigned int target_hi : 8;
  unsigned int padding : 24;
  unsigned int na;
};

struct opcode_waitopwr {
  unsigned int opcode_number : 8;
  unsigned int tolerance_lo : 24;
  unsigned int tolerance_hi : 8;
  unsigned int debounce : 8;
  unsigned int timeout : 8;
  unsigned int padding : 8;
  unsigned int na;
};

union u_opcode {
  struct opcode_raw raw;
  struct opcode_simple32 simple32;
  struct opcode_mmov mmov;
  struct opcode_axismov axismov;
  struct opcode_axispos axispos;
  struct opcode_pdelay pdelay;
  struct opcode_arg_uint8 arg_uint8;
  struct opcode_arg_uint16 arg_uint16;
  struct opcode_arg_uint24 arg_uint24;
  struct opcode_qlineacq qlineacq;
  struct opcode_qposacq qposacq;
  struct opcode_waitpos waitpos;
  struct opcode_qtimeacq qtimeacq;
  struct opcode_setopwr setopwr;
  struct opcode_waitopwr waitopwr;
};

union u_opcode opcode;

int opcode_size;
int opcode_bytecnt;
unsigned short int utmp16;
unsigned int utmp32;
short int tmp16;
int tmp32;
float tmpfloat;

int last_adcx, last_adcy;
int last_valid;

union float_conversion {
  unsigned int bits;
  float floating_point;
};

union float_conversion floatconv;

void dump_opcode()
{
  if (opcode.raw.byte0 & 0x80)
    printf( "[%02x%02x%02x%02x %02x%02x%02x%02x]: ", opcode.raw.byte0, opcode.raw.byte1, opcode.raw.byte2, opcode.raw.byte3, opcode.raw.byte4, opcode.raw.byte5, opcode.raw.byte6, opcode.raw.byte7 );
  else
    printf( "[%02x%02x%02x%02x         ]: ", opcode.raw.byte0, opcode.raw.byte1, opcode.raw.byte2, opcode.raw.byte3 );

  return;
};

void calc_delta(int new_adcx, int new_adcy)
{
  int delta_adcx;
  int delta_adcy;

  if (last_valid == 1)
  {
    delta_adcx = new_adcx - last_adcx;
    delta_adcy = new_adcy - last_adcy;
    printf( " // delta = " );
    if (delta_adcx > 0)
      printf( "+" );
    printf( "%0d, ", delta_adcx );
    if (delta_adcy > 0)
      printf( "+" );
    printf( "%0d\n", delta_adcy );
  } else {
    printf( "\n" );
  }
  last_valid = 1;
  last_adcx = new_adcx;
  last_adcy = new_adcy;
    
  return;
}

int main ( int argc, char** argv )
{
  if (argc != 2)
  {
    fprintf( stderr, "USAGE: %s <pbin file>\n", argv[0] );
    return -1;
  }

  filein = fopen( argv[1], "r" );
  if (filein == NULL)
  {
    fprintf( stderr, "%s: ERROR, cannot open %s for reading.\n", argv[1] );
    return -1;
  }

  // Initialize
  last_valid = 0;

  opcode_bytecnt = 0;
  while ((charin = fgetc( filein )) != EOF)
  {
    switch (opcode_bytecnt) {
      case 0: if (charin & 0x80)
              {
                // 8-byte opcode
                opcode_size = 8;
              } else {
                // 4-byte opcode
                opcode_size = 4;
              }
              opcode.raw.byte0 = charin;
              break;
      case 1: opcode.raw.byte1 = charin;
              break;
      case 2: opcode.raw.byte2 = charin;
              break;
      case 3: opcode.raw.byte3 = charin;
              break;
      case 4: opcode.raw.byte4 = charin;
              break;
      case 5: opcode.raw.byte5 = charin;
              break;
      case 6: opcode.raw.byte6 = charin;
              break;
      case 7: opcode.raw.byte7 = charin;
              break;
    }

    opcode_bytecnt++;
    if (opcode_bytecnt == opcode_size)
    {
      opcode_bytecnt = 0;
      dump_opcode();
      switch (opcode.raw.byte0) {
        case OPCODE_NOP:  printf( "NOP\n" );
             break;
        case OPCODE_STOP: printf( "STOP\n" );
                          return 0;
                          break;
        case OPCODE_PAUSE: printf( "PAUSE %0d\n", opcode.arg_uint24.arg );
                           break;
        case OPCODE_BREAK:  printf( "BREAK\n" );
             break;
        case OPCODE_FLUSHQ:  printf( "FLUSHQ\n" );
             break;
        case OPCODE_PDELAY:  printf( "PDELAY time_inc: %0d\n", opcode.pdelay.time_inc );
             break;
        case OPCODE_QPDELAY:  printf( "QPDELAY time_inc: %0d\n", opcode.pdelay.time_inc );
             break;
        case OPCODE_PYLON:  printf( "PYLON\n" );
             break;
        case OPCODE_QPYLON:  printf( "QPYLON\n" );
             break;
        case OPCODE_NOPYLON:  printf( "NOPYLON\n" );
             break;
        case OPCODE_QNOPYLON:  printf( "QNOPYLON\n" );
             break;
        case OPCODE_LSAFEON:  printf( "LSAFEON\n" );
             break;
        case OPCODE_SETLAYER:  printf( "SETLAYER %0d\n", opcode.arg_uint8.arg );
             break;
        case OPCODE_SETGRP:  printf( "SETGRP %0d\n", opcode.arg_uint16.arg );
             break;
        case OPCODE_FLMOV:  printf( "FLMOV %0d\n", opcode.arg_uint16.arg );
             break;
        case OPCODE_PTCOMP:  printf( "PTCOMP\n" );
             break;
        case OPCODE_PTCOMP2:  printf( "PTCOMP\n" );
             break;
        case OPCODE_SETPOS:  printf( "SETPOS\n" );
             break;
        case OPCODE_SETOPWR: utmp32 = opcode.setopwr.target_hi << 24 | opcode.setopwr.target_lo;
                             floatconv.bits = utmp32;
                             tmpfloat = floatconv.floating_point;
                             printf( "SETOPWR target: %0f\n", tmpfloat );
                             break;
        case OPCODE_WAITOPWR: utmp32 = opcode.waitopwr.tolerance_hi << 24 | opcode.waitopwr.tolerance_lo;
                              floatconv.bits = utmp32;
                              tmpfloat = floatconv.floating_point;
                              printf( "WAITOPWR tolerance: %0f debounce: %0d timeout: %0d\n", tmpfloat, opcode.waitopwr.debounce, opcode.waitopwr.timeout  );
                              break;
        case OPCODE_MMOV: utmp16 = (opcode.mmov.y_coor_hi << 8) | opcode.mmov.y_coor_lo;
                          tmp16 = (short int) utmp16;
                          tmp32 = (int) tmp16;
                          printf( "MMOV %0d, %0d +%0d ", opcode.mmov.x_coor, tmp32, opcode.mmov.time_inc );
                          calc_delta( opcode.mmov.x_coor, tmp32 );
                          break;
        case OPCODE_TIMEDAXISMOV: printf( "TIMEDAXISMOV " );
                                  if (opcode.axismov.condition == 0)
                                    printf( "GE " );
                                  else
                                    printf( "LE " );
                                  switch (opcode.axismov.axis) {
                                    case 0: printf( "X " ); break;
                                    case 1: printf( "Y " ); break;
                                    default: printf( "ERROR " ); break;
                                  }
                                  printf( "ADC count: %0d timeout: %0d\n", opcode.axismov.adc_count, opcode.axismov.timeout );
                                  break;
        case OPCODE_WAITAXISMOV: printf( "WAITAXISMOV " );
                                 if (opcode.axismov.condition == 0)
                                   printf( "GE " );
                                 else
                                   printf( "LE " );
                                 switch (opcode.axismov.axis) {
                                   case 0: printf( "X " ); break;
                                   case 1: printf( "Y " ); break;
                                   default: printf( "ERROR " ); break;
                                 }
                                 printf( "ADC count: %0d timeout: %0d\n", opcode.axismov.adc_count, opcode.axismov.timeout );
                                 break;
        case OPCODE_TIMEDAXISPOS: printf( "TIMEDAXISPOS " );
                                  switch (opcode.axispos.axis) {
                                    case 0: printf( "X " ); break;
                                    case 1: printf( "Y " ); break;
                                    default: printf( "ERROR " ); break;
                                  }
                                  printf( "ADC count: %0d tolerance: %0d timeout: %0d\n", opcode.axispos.adc_count, opcode.axispos.tolerance, opcode.axispos.timeout );
                                  break;
        case OPCODE_WAITAXISPOS: printf( "WAITAXISPOS " );
                                 switch (opcode.axispos.axis) {
                                   case 0: printf( "X " ); break;
                                   case 1: printf( "Y " ); break;
                                   default: printf( "ERROR " ); break;
                                 }
                                 printf( "ADC count: %0d tolerance: %0d timeout: %0d\n", opcode.axispos.adc_count, opcode.axispos.tolerance, opcode.axispos.timeout );
                                 break;
        case OPCODE_TIMEDPOS: utmp16 = (opcode.waitpos.y_adc_count_hi << 8) | opcode.waitpos.y_adc_count_lo;
                              tmp16 = (short int) utmp16;
                              tmp32 = (int) tmp16;
                              printf( "TIMEDPOS x_adc_count: %0d y_adc_count: %0d x_tol: %0d y_tol: %0d timeout %0d\n",
                                       opcode.waitpos.x_adc_count, tmp32, opcode.waitpos.x_count_tol, opcode.waitpos.y_count_tol, opcode.waitpos.timeout );
                              break;
        case OPCODE_WAITPOS:  utmp16 = (opcode.waitpos.y_adc_count_hi << 8) | opcode.waitpos.y_adc_count_lo;
                              tmp16 = (short int) utmp16;
                              tmp32 = (int) tmp16;
                              printf( "WAITPOS x_adc_count: %0d y_adc_count: %0d x_tol: %0d y_tol: %0d timeout %0d\n",
                                       opcode.waitpos.x_adc_count, tmp32, opcode.waitpos.x_count_tol, opcode.waitpos.y_count_tol, opcode.waitpos.timeout );
                              break;
        case OPCODE_QLINEACQ:  printf( "QLINEACQ " );
                               if (opcode.qlineacq.condition == 0)
                                 printf( "GE " );
                               else
                                 printf( "LE " );
                               if (opcode.qlineacq.axis == 0)
                                 printf( "X " );
                               else
                                 printf( "Y " );
                               printf( "Starting Count: %0d ", opcode.qlineacq.starting_count );
                               printf( "IMUL: %0d ", opcode.qlineacq.imul );
                               printf( "rep: %0d ", opcode.qlineacq.rep );
                               printf( "Count Increment: %0d\n", opcode.qlineacq.count_inc );
                               break;
        case OPCODE_QPOSACQ: utmp16 = (opcode.qposacq.y_adc_count_hi << 8) | opcode.qposacq.y_adc_count_lo;
                             tmp16 = (short int) utmp16;
                             tmp32 = (int) tmp16;
                             printf( "QPOSACQ x_adc_count: %0d y_adc_count: %0d x_tol: %0d y_tol: %0d imul %0d\n",
                                      opcode.qposacq.x_adc_count, tmp32, opcode.qposacq.x_count_tol, opcode.qposacq.y_count_tol, opcode.qposacq.imul );
                             break;
        case OPCODE_QTIMEACQ: utmp32 = opcode.qtimeacq.rep_hi << 16 | opcode.qtimeacq.rep_lo;
                              printf( "QTIMEACQ IMUL: %0d rep: %0d time_inc: %0d\n", opcode.qtimeacq.imul, utmp32, opcode.qtimeacq.time_inc );
                              break;
        case OPCODE_MMOVLINE:  printf( "MMOVLINE\n" );
             break;
        case OPCODE_EXTEND:  printf( "EXTEND\n" );
             break;
        default: printf( "ERROR, unknown opcode number: %0d\n", opcode.raw.byte0 );
                 break;
      }
    }
  }
  

  return 0;
}
