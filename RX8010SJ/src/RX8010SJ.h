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

		#define RX8010_EXT_WADA  BIT(3)
		#define RX8010_FLAG_VLF  BIT(1)
		#define RX8010_FLAG_AF   BIT(3)
		#define RX8010_FLAG_TF   BIT(4)
		#define RX8010_FLAG_UF   BIT(5)
		#define RX8010_CTRL_AIE  BIT(3)
		#define RX8010_CTRL_UIE  BIT(5)
		#define RX8010_CTRL_STOP BIT(6)
		#define RX8010_CTRL_TEST BIT(7)
		#define RX8010_ALARM_AE  BIT(7)

		struct DateTime {
			byte seconds;
			byte minutes;
			byte hours;
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
                DateTime readDateTime(void);
				void writeDateTime(DateTime dateTime);
                
            private:
                byte i2cAddress;

				byte readFromModule(byte address);
				void writeToModule(byte address, byte data);
				void writeFlag(byte address, byte pos, byte value);
				byte getValueFromBinary(byte binary, byte pos, byte val);
				byte setFortyBinary(byte binary, byte val);
				byte setTwentyBinary(byte binary, byte val);
				byte setTenBinary(byte binary, byte val);
				byte setBinary(byte binary, byte pos, byte flagVal);
        };
    }

#endif