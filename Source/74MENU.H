#define tlen0_19    18
#define tlen20_49    16
#define tlen50_99    19
#define tlen100_149    15
#define tlen150_189    22
#define tlen190_279    18
#define tlen280_599    18
#define tlen600_9000    19
#define tlen74XX    9
const char * const tlab0_19[] = { 0,tlab003B,tlab003D,tlab0043,tlab003C,tlab0038,tlab0039,tlab0039,tlab004C,tlab0046,tlab0047,tlab0040,tlab0028,tlab0041,tlab003E,tlab0038,tlab0027,tlab0039 };
const char * const tlab20_49[] = { 0,tlab003E,tlab0032,tlab003F,tlab002B,tlab002A,tlab003C,tlab0030,tlab0043,tlab003A,tlab0045,tlab0044,tlab003B,tlab003C,tlab003E,tlab0035 };
const char * const tlab50_99[] = { 0,tlab0022,tlab0023,tlab0037,tlab0019,tlab001F,tlab0008,tlab001A,tlab004A,tlab0029,tlab004B,tlab002F,tlab0020,tlab0021,tlab0072,tlab0057,tlab0053,tlab0054,tlab0010 };
const char * const tlab100_149[] = { 0,tlab001B,tlab000A,tlab0052,tlab0007,tlab0009,tlab0031,tlab0043,tlab003B,tlab0073,tlab0049,tlab0017,tlab003E,tlab0036,tlab0033 };
const char * const tlab150_189[] = { 0,tlab005C,tlab0034,tlab0058,tlab0024,tlab0048,tlab0018,tlab0062,tlab0063,tlab0064,tlab0016,tlab006D,tlab0077,tlab000C,tlab000D,tlab006E,tlab0013,tlab004F,tlab004E,tlab001E,tlab0000,tlab000E };
const char * const tlab190_279[] = { 0,tlab0003,tlab0067,tlab0055,tlab0056,tlab0004,tlab005D,tlab005E,tlab0076,tlab005F,tlab0074,tlab005B,tlab0068,tlab0059,tlab0069,tlab000B,tlab0026,tlab0012 };
const char * const tlab280_599[] = { 0,tlab000F,tlab0060,tlab005A,tlab0014,tlab0071,tlab0070,tlab0050,tlab002C,tlab006A,tlab0051,tlab0025,tlab0065,tlab0042,tlab001C,tlab001D,tlab006B,tlab0066 };
const char * const tlab600_9000[] = { 0,tlab0075,tlab0001,tlab006F,tlab0061,tlab006C,tlab003B,tlab0043,tlab003B,tlab0038,tlab0039,tlab0046,tlab0040,tlab0028,tlab003E,tlab0045,tlab004D,tlab004C,tlab0011 };
const char * const titem0_19[] = { "#$","00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16" };
const char * const titem20_49[] = { "#$","20","21","22","23","25","26","27","28","30","32","33","37","38","40","42" };
const char * const titem50_99[] = { "#$","50","51","53","54","55","64","72","74","75","77","82","83","85","86","90","92","93","95" };
const char * const titem100_149[] = { "#$","107","109","112","113","114","125","128","132","136","138","139","140","145","148" };
const char * const titem150_189[] = { "#$","151","152","153","154","155","157","160","161","163","164","165","166","168","169","170","173","174","175","180","181","184" };
const char * const titem190_279[] = { "#$","190","191","192","193","198","240","241","243","244","245","251","253","257","258","259","273","279" };
const char * const titem280_599[] = { "#$","280","295","298","299","352","353","367","368","373","374","377","393","452","465","466","573","574" };
const char * const titem600_9000[] = { "#$","640","643","670","873","874","1000","1002","1003","1004","1005","1008","1010","1011","1020","1032","1034","1035","4520" };
const MENU tmnu0_19 = { titem0_19, 0x80, tlen0_19, (MENU **) tlab0_19 };
const MENU tmnu20_49 = { titem20_49, 0x80, tlen20_49, (MENU **) tlab20_49 };
const MENU tmnu50_99 = { titem50_99, 0x80, tlen50_99, (MENU **) tlab50_99 };
const MENU tmnu100_149 = { titem100_149, 0x80, tlen100_149, (MENU **) tlab100_149 };
const MENU tmnu150_189 = { titem150_189, 0x80, tlen150_189, (MENU **) tlab150_189 };
const MENU tmnu190_279 = { titem190_279, 0x80, tlen190_279, (MENU **) tlab190_279 };
const MENU tmnu280_599 = { titem280_599, 0x80, tlen280_599, (MENU **) tlab280_599 };
const MENU tmnu600_9000 = { titem600_9000, 0x80, tlen600_9000, (MENU **) tlab600_9000 };
const char * const titem74XX[] = { "#$","0+","20+","50+","100+","150+","190+","280+","600+"};
const MENU * const tlab74XX[] = { 0,&tmnu0_19,&tmnu20_49,&tmnu50_99,&tmnu100_149,&tmnu150_189,&tmnu190_279,&tmnu280_599,&tmnu600_9000};
const MENU mnu74XX = { titem74XX, 0, tlen74XX, tlab74XX  };
