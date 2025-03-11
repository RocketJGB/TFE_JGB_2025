


extern int mode;
extern char type;
extern unsigned int def_value;

void C_command(void);

void I_command(void);

void Z_command(void);

void Reset(void);

void Set_servo(int chan, int value, int time);

unsigned int Register (int chan);

void init_PCA9685();

/*int Mode1(void);
int Mode2(void);
int Mode3(void);*/