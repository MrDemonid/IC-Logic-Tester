#define clen4000_4499    20
#define clen4500_9000    5
#define clen4XXX    3
const char * const clab4000_4499[] = { 0,clab0015,clab000A,clab0014,clab000D,clab000E,clab0002,clab0005,clab000F,clab000C,clab000B,clab0008,clab0003,clab0001,clab0007,clab0016,clab0005,clab0016,clab0014,clab0000 };
const char * const clab4500_9000[] = { 0,clab0017,clab0009,clab0012,clab0013 };
const char * const citem4000_4499[] = { "#$","4001","4002","4011","4012","4013","4015","4016","4019","4023","4025","4027","4028","4028","4043","4049","4066","4069","4093","4094" };
const char * const citem4500_9000[] = { "#$","4502","4503","4516","4520" };
const MENU cmnu4000_4499 = { citem4000_4499, 0x80, clen4000_4499, (MENU **) clab4000_4499 };
const MENU cmnu4500_9000 = { citem4500_9000, 0x80, clen4500_9000, (MENU **) clab4500_9000 };
const char * const citem4XXX[] = { "#$","4000+","4500+"};
const MENU * const clab4XXX[] = { 0,&cmnu4000_4499,&cmnu4500_9000};
const MENU mnu4XXX = { citem4XXX, 0, clen4XXX, clab4XXX  };
