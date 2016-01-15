
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include "sstream"

using namespace std;    // Or using std::string;
#define BUTTONOK 10
#define BUTTONCOPY 10
HINSTANCE g_inst;
HWND Cateto1, Cateto2, ButtonOk, Saida2, ButtonCopy, ButtonClean;
HWND Cateto1_bkp, Cateto2_bkp, Saida2_bkp;
char Cat2[100] = "0",Cat1[20] = "0", Hipot[20] = "0", resultado[20]="0";
int CatetoA, CatetoB;
double hipotenusa;


//verify if string is a integer or a string to '' option in output  source: http://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
inline bool isInteger(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}


// reverses a string 'str' of length 'len'
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

 // Converts a given integer x to string str[].  d is the number
 // of digits required in output. If d is more than the number
 // of digits in x, then 0s are added at the beginning.
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

// Converts a floating point number to string.
void ftoa(float n, char *res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}

// Abaixo estrutura dentro da Janela

void DrawComponents(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

CreateWindowEx (
0,
"STATIC",
"List of Values",
WS_VISIBLE|WS_CHILD|WS_BORDER,
30, 20, 90, 19,
hwnd,
NULL,
g_inst,
NULL
);
Cateto1 = CreateWindowEx (
WS_EX_CLIENTEDGE,
"EDIT",
"",
WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_WANTRETURN|WS_VSCROLL,
30, 50, 250, 100,
hwnd,
NULL,
g_inst,
NULL
);
SendMessage(Cateto1, EM_LIMITTEXT, 0, 0L);
CreateWindowEx (
0,
"STATIC",
"Field",
WS_CHILD|WS_VISIBLE|WS_BORDER,
300, 20, 36, 19,
hwnd,
NULL,
g_inst,
NULL
);

Cateto2 = CreateWindowEx (
WS_EX_CLIENTEDGE,
"EDIT",
"",
WS_CHILD|WS_VISIBLE|ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN,
300, 50, 150, 50,
hwnd,
NULL,
g_inst,
NULL
);

ButtonOk = CreateWindowEx (
0,
"BUTTON",
"Generate",
WS_VISIBLE|WS_CHILD,
300, 130, 90, 20,
hwnd,
(HMENU)BUTTONOK,
g_inst,
NULL
);


ButtonCopy = CreateWindowEx (
0,
"BUTTON",
"Output Copy",
WS_VISIBLE|WS_CHILD,
300, 200, 110, 20,
hwnd,
(HMENU)BUTTONCOPY,
g_inst,
NULL
);

ButtonClean = CreateWindowEx (
0,
"BUTTON",
"Clear All",
WS_VISIBLE|WS_CHILD,
300, 230, 110, 20,
hwnd,
(HMENU)BUTTONCOPY,
g_inst,
NULL
);

CreateWindowEx (
0,
"STATIC",
"Output",
WS_VISIBLE|WS_CHILD|WS_BORDER,
30, 170, 46, 20,
hwnd,
NULL,
g_inst,
NULL
);

Saida2 = CreateWindowEx (
WS_EX_CLIENTEDGE,
"EDIT",
"",
WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_AUTOVSCROLL|ES_WANTRETURN|WS_VSCROLL,
30, 200, 250, 90,
hwnd,
NULL,
g_inst,
NULL
);


}

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

// WinMain, a função main em janelas (janela principal)
int WINAPI WinMain (HINSTANCE hThisInstance,
HINSTANCE hPrevInstance,
LPSTR lpszArgument,
int nFunsterStil)
{

HWND hwnd;      // O objeto para indicar a janela
MSG messages;   // Objeto mensagem do Windows para controlar o acionamento de botões
WNDCLASSEX wincl;   // O objeto para criar a janela

wincl.hInstance = hThisInstance;
wincl.lpszClassName = "WindowsAppl"; // Nome da classe da janela
wincl.lpfnWndProc = WindowProcedure;
wincl.style = CS_DBLCLKS;
wincl.cbSize = sizeof (WNDCLASSEX);
wincl.hIcon = LoadIcon (NULL, IDI_ASTERISK);
wincl.hCursor = LoadCursor (NULL, IDC_ARROW);          //Cursor default
wincl.lpszMenuName = NULL;
wincl.cbClsExtra = 0;
wincl.cbWndExtra = 0;
wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;       // Cor cinza de background

if (!RegisterClassEx (&wincl))  // Se o registro da classe foi mal-sucedido return 0
return 0;

hwnd = CreateWindowEx (               // Criação da janela
0,
"WindowsAppl",                 // Indicação da classe da janela
"Query SQL using IN and list v1.2",                 // Nome da janela
WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,           // Estilo da janela sem maximizar e alterar tamanho WS_OVERLAPPEDWINDOW -- user total control
350,                               // x em relação ao desktop
250,                                 // y em relação ao desktop
480,                                  // largura da janela
400,                               // altura da janela
HWND_DESKTOP,
NULL,
hThisInstance,
NULL
);

ShowWindow (hwnd, nFunsterStil);                // Mostrar a janela

while (GetMessage (&messages, NULL, 0, 0)) // Recuperar mensagens
{
TranslateMessage(&messages);
DispatchMessage(&messages);
}

return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
switch (message)
{
case WM_CREATE:
	DrawComponents(hwnd,message,wParam,lParam);
break;
// Quando é executado o clique do mouse no botão
case WM_COMMAND:
     if ((HIWORD(wParam) == BN_CLICKED))
{
     if ((HWND)lParam == ButtonOk)
{
int len = SendMessage((HWND)Cateto1, EM_GETLINECOUNT, 0, 0); // conta o numero de linhas das variaveis
static char saida01[5000000],saida02[5000000], saida11[3000000];
static char saida02_bkp[50000] = "";
char *saida011;
char *saida012;
GetWindowText((HWND)Cateto1,saida01,2000000); // pega o texto e transfere para variavel que posso controlar
GetWindowText((HWND)Cateto2,saida11,1000000);// pega o texto e transfere para variavel que posso controlar
saida011 = const_cast<char*>(saida01);// Converte a variavel armazenada no GetWindowText para ponteiro (endereço de memoria)
saida012 = const_cast<char*>(saida11);// Converte a variavel armazenada no GetWindowText para ponteiro (endereço de memoria)
char *linha[9000]; // variavel que ira contar por linha cada elemento inserido
int i=1,j=1;
linha [i]= strtok(saida011, "\r\n");
   while (i<len)
   {
   i++;
      /** Organizando  por linhas as variaveis de entrada repetidas */
    linha[i] = strtok (NULL,"\r\n");
        if (linha[i]==NULL)
    {
    MessageBox( hwnd, "Can't make the query with blank spaces between lines!", "Error in the list", MB_OK );
    exit(1);
    }
   }
/** Criando o comando de saida */
   while (j<=len)
   {
   if (j==1)
   {
    sprintf(saida02+strlen(saida02),"\"%s\" IN (",saida012);
    }
   if (j>=1)
   {

    if (isInteger(linha[j]))
    {sprintf(saida02+strlen(saida02),"%s",linha[j]);}
    else
        {sprintf(saida02+strlen(saida02),"'%s'",linha[j]);}
    }
   if (j<len)
   {
    sprintf(saida02+strlen(saida02),",");
    }
    else
    {
     sprintf(saida02+strlen(saida02),")",saida012,linha[j]);
     }

   j++;
   }



//SAida na janela final das respostas;
SetWindowTextA( Saida2, saida02 );
strncpy ( saida02, saida02_bkp, sizeof(saida02_bkp) );
}
 if ((HWND)lParam == ButtonCopy)
 {
 char *output_ponteiro;
 char *vazio;
 char output_char[9000];
 char vazio_char[20]="";
 GetWindowText((HWND)Saida2,output_char,20000);
 output_ponteiro = const_cast<char*>(output_char);// Converte a variavel armazenada no GetWindowText para ponteiro (endereço de memoria)
 vazio = const_cast<char*>(vazio_char);// Converte a variavel armazenada no GetWindowText para ponteiro (endereço de memoria)
 const size_t len = strlen(output_ponteiro) + 1;
    HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), output_ponteiro, len);
    GlobalUnlock(hMem);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
 MessageBox( hwnd, "Result was copy to memory", "Ctrl+C", MB_OK );
 }
 if ((HWND)lParam == ButtonClean)
 {
 char *vazio;
 char vazio_char[20]="";
 vazio = const_cast<char*>(vazio_char);// Converte a variavel armazenada no GetWindowText para ponteiro (endereço de memoria)
 SetWindowTextA( Saida2, vazio );
 SetWindowTextA( Cateto1, vazio );
 SetWindowTextA( Cateto2, vazio );
 MessageBox( hwnd, "Clean all values !", "Clean all values", MB_OK );

 }
}


break;
case WM_DESTROY:   // Destruição da janela
PostQuitMessage (0);
break;
default:
return DefWindowProc (hwnd, message, wParam, lParam);
}
return 0;
}


