#ifndef RX8010SJ_ADAPTER
    #define RX8010SJ_ADAPTER

    #include "Arduino.h"
	#include "Wire.h"

    namespace RX8010SJ {
		#define RX8010_SEC     0x10
		#define RX8010_MIN     0x11
		#define RX8010_HOUR    0x12
		#define RX8010_WDAY    0x13
		#define RX8010_MDAY    0x14
		#define RX8010_MONTH   0x15
		#define RX8010_YEAR    0x16
		#define RX8010_RESV17  0x17
		#define RX8010_ALMIN   0x18
		#define RX8010_ALHOUR  0x19
		#define RX8010_ALWDAY  0x1A
		#define RX8010_TCOUNT0 0x1B
		#define RX8010_TCOUNT1 0x1C
		#define RX8010_EXT     0x1D
		#define RX8010_FLAG    0x1E
		#define RX8010_CTRL    0x1F
		/* 0x20 to 0x2F are user registers */
		#define RX8010_RESV30  0x30
		#define RX8010_RESV31  0x31
		#define RX8010_IRQ     0x32

		/* Default values for reseting the module */
		#define RX8010_ADDR17_DEF_VAL 0xD8
		#define RX8010_ADDR30_DEF_VAL 0x00
		#define RX8010_ADDR31_DEF_VAL 0x08
		#define RX8010_ADDR31_DEF_VAL 0x08
		#define RX8010_IRQ_DEF_VAL    0x04
		#define RX8010_CTRL_DEF_VAL   0x04

		#define RX8010_VLF_POS   1
		#define RX8010_TE_POS    4
		#define RX8010_TSEL0_POS 0
		#define RX8010_TSEL1_POS 1
		#define RX8010_TSEL2_POS 2
		#define RX8010_TIE_POS   4
		#define RX8010_TMPIN_POS 2
		#define RX8010_TF_POS    4
		#define RX8010_STOP_POS  6
		#define RX8010_TSTP_POS  2
		#define RX8010_AIE_POS   3
		#define RX8010_WADA_POS  3
		#define RX8010_AF_POS    3

		#define RX8010_AL_DISABLED 0b10000000

		struct DateTime {
			byte second;
			byte minute;
			byte hour;
			byte dayOfWeek; // Has to be set
			byte dayOfMonth;
			byte month;
			byte year; // Two digits
		};

        class Adapter {
            public:
                // Constructor/destructor
                Adapter(byte i2cSlaveAddr);
                virtual ~Adapter();

                // Methods
				bool initAdapter();
				bool initModule();
				void resetModule(void);

				// Calendar
                DateTime readDateTime(void);
				void writeDateTime(DateTime dateTime);

				// Fixed cycle interrupt
				void setFCTCounter(uint16_t counterValue, byte tsel);
				uint16_t getFCTCounter();
				void setFCTOutput(byte output);
				void enableFCT();
				void disableFCT();
				bool checkFCT();

				// Alarm
				void setAlarm(DateTime alarmTime, bool alarmMode);
				void enableAlarm();
				void disableAlarm();
				bool checkAlarm();

            private:
                byte i2cAddress;

				byte readFromModule(byte address);
				void writeToModule(byte address, byte data);
				void writeFlag(byte address, byte pos, byte value);
				byte getSingleBit(byte binary);
				byte getValueFromBinary(byte binary, byte pos);
				byte getValueFromBinary(byte binary, byte pos, byte val);
				byte sumValueFromBinary(byte binary, byte length);
				byte setEightyBinary(byte binary, byte val);
				byte setFortyBinary(byte binary, byte val);
				byte setTwentyBinary(byte binary, byte val);
				byte setTenBinary(byte binary, byte val);
				byte setBinary(byte binary, byte pos, byte flagVal);
        };
    }

#endif