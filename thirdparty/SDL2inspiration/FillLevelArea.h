#ifndef FILLLEVELAREA_H_INCLUDED
#define FILLLEVELAREA_H_INCLUDED


// Bei Levelgr��e 1000 x 1000 werden bis zu 2.500.000 Elemente ben�tigt
#define FILLSTACK_SIZE      (MAX_LEVEL_W * MAX_LEVEL_H * 3) // aktuell 3.000.000 Elemente

// F�r die iterative F�llroutine
typedef struct {
int nX;
int nY;
} KOORDS;


typedef struct {
KOORDS Koords[FILLSTACK_SIZE];
uint32_t uP;
uint32_t uMaxP;
} FILLSTACK;

void InitFillStack(void);
int PushFillstack(int nX, int nY);
int PopFillstack(int *pnX, int *pnY);
int FillLevelArea(int nX,int nY,uint16_t uFillElement,uint16_t uGroundElement);


#endif // FILLLEVELAREA_H_INCLUDED
