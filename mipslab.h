/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr );

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare bitmap array containing icon */
extern const uint8_t const icon[128];
/* Declare text buffer for display output */
extern char textbuffer[4][16];

extern uint8_t screen[128 * 32];


// setup
int config(void);
int configPipes(void);
int configBird(void);
//void configBTNnSW();
void user_isr(void);


// Display 
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);
void clear_text();


// helper funcs
void draw_pixel(int x, int y, uint8_t *array);
void clearBytes();
void randSeed(int oneOrTwo);
int randSetPipe(int oneOrTwo, int difficulty);
void draw_pipe_one();
void draw_pipe_two();

//buttons and switches
int getbtns();
int getsw();

// game funcs
void menu();
void gameOverMenu();
void highScoreMenu();
void gameplay();


//Name
void nameInput();