// Code::Blocks 13.12 GNU GCC Compiler

#include <windows.h>
#include <gdiplus.h>
#include <vector>

using namespace Gdiplus;

struct figura
{
    char typ;
    int lewy, prawy, gora, dol;
};

HINSTANCE hInstance;
LPSTR NazwaKlasy = "Klasa okna";
MSG komunikat;
Point lina_p, lina_k;
std::vector<figura> figury;
std::vector<figura> wieza;
RECT obszar={350,127,1000,659};
bool ladunek=false;
int indeks=-1, wieza_wys=0;

HWND hwnd;

LRESULT CALLBACK WndProc(HWND hwnd, UINT komunikat, WPARAM wParam, LPARAM lParam);
void Rysowanie(HDC hdc);
void Ruch_lewo(HDC hdc, PAINTSTRUCT ps);
void Ruch_prawo(HDC hdc, PAINTSTRUCT ps);
void Ruch_dol(HDC hdc, PAINTSTRUCT ps);
void Ruch_gora(HDC hdc, PAINTSTRUCT ps);
void Ladunek(HDC hdc, PAINTSTRUCT ps);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    //klasa dla okna
    WNDCLASSEX wc;

    wc.cbSize = sizeof( WNDCLASSEX );
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
    wc.lpszMenuName = NULL;
    wc.lpszClassName = NazwaKlasy;
    wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );

    if( !RegisterClassEx( & wc ) )
    {
        MessageBox( NULL, "BLAD !", "KOMUNIKAT", MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }

    //stworzenie okna glownego
    hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, NazwaKlasy, "Symulator dzwigu", WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 1300, 750, NULL, NULL, hInstance, NULL );
    if( hwnd == NULL )
    {
        MessageBox( NULL, "BLAD !", "KOMUNIKAT", MB_ICONEXCLAMATION | MB_OK );
        return 1;
    }

    // Pokaz okno
    ShowWindow( hwnd, nCmdShow );
    UpdateWindow( hwnd );

    //pobieranie komunikatow
    while( GetMessage( & komunikat, NULL, 0, 0 ) )
    {
        TranslateMessage(&komunikat);
        DispatchMessage(&komunikat);
    }
    return komunikat.wParam;

    Gdiplus::GdiplusShutdown(gdiplusToken);

    return 0;
}

//obsluga komunikatow
LRESULT CALLBACK WndProc(HWND hwnd, UINT komunikat, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;

    switch(komunikat)
    {
        case WM_KEYDOWN:
        {
            switch((int)wParam)
            {
            case VK_LEFT:
                Ruch_lewo(hdc,ps);
                break;
            case VK_RIGHT:
                Ruch_prawo(hdc,ps);
                break;
            case VK_UP:
                Ruch_gora(hdc,ps);
                break;
            case VK_DOWN:
                Ruch_dol(hdc,ps);
                break;
            case VK_SPACE:
                Ladunek(hdc,ps);
                break;
            }
        }
        break;

        case WM_PAINT:
            hdc=BeginPaint(hwnd, &ps);
            Rysowanie(hdc);
            EndPaint(hwnd,&ps);
            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

            default:
            return DefWindowProc(hwnd, komunikat, wParam, lParam);
    }

    return 0;
}

void Rysowanie(HDC hdc)
{
    //Inicjalizacja dzwigu
    Graphics grafika(hdc);
    Pen zolty(Color(255,255,220,0), 3);
    Pen czarny(Color(255,0,0,0), 3);
    SolidBrush beton(Color(255,130,130,130));
    grafika.DrawRectangle(&zolty,200,10,25,600);
    grafika.DrawRectangle(&zolty,100,100,850,25);
    for(int i=610;i>10;i-=30)
    {
        grafika.DrawLine(&zolty,200,i,225,i-30);
        grafika.DrawLine(&zolty,225,i,200,i-30);
    }
    for(int i=100;i<950;i+=25)
    {
        grafika.DrawLine(&zolty,i,100,i+25,125);
        grafika.DrawLine(&zolty,i,125,i+25,100);
    }
    grafika.FillRectangle(&beton,10,60,90,105);
    grafika.FillRectangle(&beton,75,610,275,50);
    grafika.DrawLine(&czarny,213,10,900,100);
    grafika.DrawLine(&czarny,213,10,600,100);
    grafika.DrawLine(&czarny,0,660,10000,660);
    grafika.DrawLine(&czarny,213,10,100,100);
    grafika.DrawLine(&czarny,600,127,600,300);
    lina_p.X=600;
    lina_p.Y=127;
    lina_k.X=600;
    lina_k.Y=300;

    //dodanie poczatkowych figur
    figura figura;
    grafika.DrawRectangle(&czarny,890,600,60,60);
    figura.typ='K';
    figura.dol=660;
    figura.gora=600;
    figura.lewy=890;
    figura.prawy=950;
    figury.push_back(figura);
    Point point1(780,660);
    Point point2(840,660);
    Point point3(810,600);
    Point points[3] = {point1, point2, point3};
    Point* pPoints=points;
    grafika.DrawPolygon(&czarny,pPoints,3);
    figura.typ='T';
    figura.dol=660;
    figura.gora=600;
    figura.lewy=780;
    figura.prawy=840;
    figury.push_back(figura);
    grafika.DrawEllipse(&czarny,670,600,60,60);
    figura.typ='O';
    figura.dol=660;
    figura.gora=600;
    figura.lewy=670;
    figura.prawy=730;
    figury.push_back(figura);
}

void Ruch_lewo(HDC hdc, PAINTSTRUCT ps)
{
    if(ladunek and ((!wieza_wys and lina_k.X>400) or (wieza_wys and ((lina_k.X-30>wieza[wieza_wys-1].prawy and lina_k.Y+60>wieza[wieza_wys-1].gora) or (lina_k.Y+60<=wieza[wieza_wys-1].gora)))))
    {
        InvalidateRect(hwnd,&obszar,TRUE);
        hdc=BeginPaint(hwnd,&ps);

        Graphics grafika(hdc);
        Pen czarny(Color(255,0,0,0), 3);
        SolidBrush pomaranczowy(Color(255,252,100,3));
        SolidBrush czarny2(Color(255,0,0,0));
        grafika.DrawLine(&czarny,lina_p.X-2,lina_p.Y,lina_k.X-2,lina_k.Y);
        lina_p.X-=2;
        lina_k.X-=2;
        figury[indeks].lewy-=2;
        figury[indeks].prawy=figury[indeks].lewy+60;

        for(int i=0;i<figury.size();i++)
        {
            if(figury[i].typ=='K')
            {
                grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='O')
            {
                grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='T')
            {
                Point point1(figury[i].lewy,figury[i].dol);
                Point point2(figury[i].prawy,figury[i].dol);
                Point point3(figury[i].lewy+30,figury[i].gora);
                Point points[3] = {point1, point2, point3};
                Point* pPoints=points;
                grafika.DrawPolygon(&czarny,pPoints,3);
            }
        }
        for(int i=0;i<wieza.size();i++)
            grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

        if(wieza_wys==3)
        {
            grafika.FillEllipse(&czarny2,wieza[0].lewy+25,wieza[0].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[1].lewy+25,wieza[1].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+10,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+40,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&pomaranczowy,wieza[2].lewy+25,wieza[2].gora+30,10,30);
        }

        EndPaint(hwnd,&ps);
    }
    else if(!ladunek and lina_p.X>400)
    {
        InvalidateRect(hwnd,&obszar,TRUE);
        hdc=BeginPaint(hwnd,&ps);

        Graphics grafika(hdc);
        Pen czarny(Color(255,0,0,0), 3);
        SolidBrush pomaranczowy(Color(255,252,100,3));
        SolidBrush czarny2(Color(255,0,0,0));
        grafika.DrawLine(&czarny,lina_p.X-2,lina_p.Y,lina_k.X-2,lina_k.Y);
        lina_p.X-=2;
        lina_k.X-=2;

        for(int i=0;i<figury.size();i++)
        {
            if(figury[i].typ=='K')
            {
                grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='O')
            {
                grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='T')
            {
                Point point1(figury[i].lewy,figury[i].dol);
                Point point2(figury[i].prawy,figury[i].dol);
                Point point3(figury[i].lewy+30,figury[i].gora);
                Point points[3] = {point1, point2, point3};
                Point* pPoints=points;
                grafika.DrawPolygon(&czarny,pPoints,3);
            }
        }
        for(int i=0;i<wieza.size();i++)
            grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

        if(wieza_wys==3)
        {
            grafika.FillEllipse(&czarny2,wieza[0].lewy+25,wieza[0].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[1].lewy+25,wieza[1].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+10,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+40,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&pomaranczowy,wieza[2].lewy+25,wieza[2].gora+30,10,30);
        }

        EndPaint(hwnd,&ps);
    }
}

void Ruch_prawo(HDC hdc, PAINTSTRUCT ps)
{
    if(ladunek and ((lina_k.Y<=600 and lina_k.X<750) or (lina_k.Y<=538 and lina_k.X>=750 and lina_p.X<920)))
    {
        InvalidateRect(hwnd,&obszar,TRUE);
        hdc=BeginPaint(hwnd,&ps);

        Graphics grafika(hdc);
        Pen czarny(Color(255,0,0,0), 3);
        SolidBrush pomaranczowy(Color(255,252,100,3));
        SolidBrush czarny2(Color(255,0,0,0));
        grafika.DrawLine(&czarny,lina_p.X+2,lina_p.Y,lina_k.X+2,lina_k.Y);
        lina_p.X+=2;
        lina_k.X+=2;
        figury[indeks].lewy+=2;
        figury[indeks].prawy=figury[indeks].lewy+60;

        for(int i=0;i<figury.size();i++)
        {
            if(figury[i].typ=='K')
            {
                grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='O')
            {
                grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='T')
            {
                Point point1(figury[i].lewy,figury[i].dol);
                Point point2(figury[i].prawy,figury[i].dol);
                Point point3(figury[i].lewy+30,figury[i].gora);
                Point points[3] = {point1, point2, point3};
                Point* pPoints=points;
                grafika.DrawPolygon(&czarny,pPoints,3);
            }
        }
        for(int i=0;i<wieza.size();i++)
            grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

        if(wieza_wys==3)
        {
            grafika.FillEllipse(&czarny2,wieza[0].lewy+25,wieza[0].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[1].lewy+25,wieza[1].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+10,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+40,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&pomaranczowy,wieza[2].lewy+25,wieza[2].gora+30,10,30);
        }

        EndPaint(hwnd,&ps);
    }
    else if(!ladunek and lina_p.X<920)
    {
        InvalidateRect(hwnd,&obszar,TRUE);
        hdc=BeginPaint(hwnd,&ps);

        Graphics grafika(hdc);
        Pen czarny(Color(255,0,0,0), 3);
        SolidBrush pomaranczowy(Color(255,252,100,3));
        SolidBrush czarny2(Color(255,0,0,0));
        grafika.DrawLine(&czarny,lina_p.X+2,lina_p.Y,lina_k.X+2,lina_k.Y);
        lina_p.X+=2;
        lina_k.X+=2;

        for(int i=0;i<figury.size();i++)
        {
            if(figury[i].typ=='K')
            {
                grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='O')
            {
                grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='T')
            {
                Point point1(figury[i].lewy,figury[i].dol);
                Point point2(figury[i].prawy,figury[i].dol);
                Point point3(figury[i].lewy+30,figury[i].gora);
                Point points[3] = {point1, point2, point3};
                Point* pPoints=points;
                grafika.DrawPolygon(&czarny,pPoints,3);
            }
        }
        for(int i=0;i<wieza.size();i++)
            grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

        if(wieza_wys==3)
        {
            grafika.FillEllipse(&czarny2,wieza[0].lewy+25,wieza[0].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[1].lewy+25,wieza[1].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+10,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+40,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&pomaranczowy,wieza[2].lewy+25,wieza[2].gora+30,10,30);
        }

        EndPaint(hwnd,&ps);
    }
}

void Ruch_dol(HDC hdc, PAINTSTRUCT ps)
{
    if(ladunek and ((lina_k.Y<600 and lina_k.X<750) or (lina_k.Y<538 and lina_k.X>=750)))
    {
        InvalidateRect(hwnd,&obszar,TRUE);
        hdc=BeginPaint(hwnd,&ps);

        Graphics grafika(hdc);
        Pen czarny(Color(255,0,0,0), 3);
        SolidBrush pomaranczowy(Color(255,252,100,3));
        SolidBrush czarny2(Color(255,0,0,0));
        grafika.DrawLine(&czarny,lina_p.X,lina_p.Y,lina_k.X,lina_k.Y+2);
        lina_k.Y+=2;
        figury[indeks].gora+=2;
        figury[indeks].dol+=2;

        for(int i=0;i<figury.size();i++)
        {
            if(figury[i].typ=='K')
            {
                grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='O')
            {
                grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='T')
            {
                Point point1(figury[i].lewy,figury[i].dol);
                Point point2(figury[i].prawy,figury[i].dol);
                Point point3(figury[i].lewy+30,figury[i].gora);
                Point points[3] = {point1, point2, point3};
                Point* pPoints=points;
                grafika.DrawPolygon(&czarny,pPoints,3);
            }
        }
        for(int i=0;i<wieza.size();i++)
            grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

        if(wieza_wys==3)
        {
            grafika.FillEllipse(&czarny2,wieza[0].lewy+25,wieza[0].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[1].lewy+25,wieza[1].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+10,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+40,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&pomaranczowy,wieza[2].lewy+25,wieza[2].gora+30,10,30);
        }

        EndPaint(hwnd,&ps);
    }
    else if(!ladunek and lina_k.Y<610)
    {
        InvalidateRect(hwnd,&obszar,TRUE);
        hdc=BeginPaint(hwnd,&ps);

        Graphics grafika(hdc);
        Pen czarny(Color(255,0,0,0), 3);
        SolidBrush pomaranczowy(Color(255,252,100,3));
        SolidBrush czarny2(Color(255,0,0,0));
        grafika.DrawLine(&czarny,lina_p.X,lina_p.Y,lina_k.X,lina_k.Y+2);
        lina_k.Y+=2;

        for(int i=0;i<figury.size();i++)
        {
            if(figury[i].typ=='K')
            {
                grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='O')
            {
                grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='T')
            {
                Point point1(figury[i].lewy,figury[i].dol);
                Point point2(figury[i].prawy,figury[i].dol);
                Point point3(figury[i].lewy+30,figury[i].gora);
                Point points[3] = {point1, point2, point3};
                Point* pPoints=points;
                grafika.DrawPolygon(&czarny,pPoints,3);
            }
        }
        for(int i=0;i<wieza.size();i++)
            grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

        if(wieza_wys==3)
        {
            grafika.FillEllipse(&czarny2,wieza[0].lewy+25,wieza[0].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[1].lewy+25,wieza[1].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+10,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+40,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&pomaranczowy,wieza[2].lewy+25,wieza[2].gora+30,10,30);
        }

        EndPaint(hwnd,&ps);
    }
}

void Ruch_gora(HDC hdc, PAINTSTRUCT ps)
{
    if(ladunek and lina_k.Y>150)
    {
        InvalidateRect(hwnd,&obszar,TRUE);
        hdc=BeginPaint(hwnd,&ps);

        Graphics grafika(hdc);
        Pen czarny(Color(255,0,0,0), 3);
        SolidBrush pomaranczowy(Color(255,252,100,3));
        SolidBrush czarny2(Color(255,0,0,0));
        grafika.DrawLine(&czarny,lina_p.X,lina_p.Y,lina_k.X,lina_k.Y-2);
        lina_k.Y-=2;
        figury[indeks].gora-=2;
        figury[indeks].dol-=2;

        for(int i=0;i<figury.size();i++)
        {
            if(figury[i].typ=='K')
            {
                grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='O')
            {
                grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='T')
            {
                Point point1(figury[i].lewy,figury[i].dol);
                Point point2(figury[i].prawy,figury[i].dol);
                Point point3(figury[i].lewy+30,figury[i].gora);
                Point points[3] = {point1, point2, point3};
                Point* pPoints=points;
                grafika.DrawPolygon(&czarny,pPoints,3);
            }
        }
        for(int i=0;i<wieza.size();i++)
            grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

        if(wieza_wys==3)
        {
            grafika.FillEllipse(&czarny2,wieza[0].lewy+25,wieza[0].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[1].lewy+25,wieza[1].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+10,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+40,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&pomaranczowy,wieza[2].lewy+25,wieza[2].gora+30,10,30);
        }

        EndPaint(hwnd,&ps);
    }
    else if(!ladunek and lina_k.Y>150)
    {
        InvalidateRect(hwnd,&obszar,TRUE);
        hdc=BeginPaint(hwnd,&ps);

        Graphics grafika(hdc);
        Pen czarny(Color(255,0,0,0), 3);
        SolidBrush pomaranczowy(Color(255,252,100,3));
        SolidBrush czarny2(Color(255,0,0,0));
        grafika.DrawLine(&czarny,lina_p.X,lina_p.Y,lina_k.X,lina_k.Y-2);
        lina_k.Y-=2;

        for(int i=0;i<figury.size();i++)
        {
            if(figury[i].typ=='K')
            {
                grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='O')
            {
                grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
            }
            else if(figury[i].typ=='T')
            {
                Point point1(figury[i].lewy,figury[i].dol);
                Point point2(figury[i].prawy,figury[i].dol);
                Point point3(figury[i].lewy+30,figury[i].gora);
                Point points[3] = {point1, point2, point3};
                Point* pPoints=points;
                grafika.DrawPolygon(&czarny,pPoints,3);
            }
        }
        for(int i=0;i<wieza.size();i++)
            grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

        if(wieza_wys==3)
        {
            grafika.FillEllipse(&czarny2,wieza[0].lewy+25,wieza[0].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[1].lewy+25,wieza[1].gora+25,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+10,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&czarny2,wieza[2].lewy+40,wieza[2].gora+15,10,10);
            grafika.FillEllipse(&pomaranczowy,wieza[2].lewy+25,wieza[2].gora+30,10,30);
        }

        EndPaint(hwnd,&ps);
    }
}

void Ladunek(HDC hdc, PAINTSTRUCT ps)
{
    if(ladunek)     //jesli jest zaczepiony ladunek
    {
        if(wieza_wys==0 and figury[indeks].prawy<620 and figury[indeks].dol>655)    //jesli jeszcze nie ma zadnej wiezy
        {
            InvalidateRect(hwnd,&obszar,TRUE);
            hdc=BeginPaint(hwnd,&ps);

            Graphics grafika(hdc);
            Pen czarny(Color(255,0,0,0), 3);

            grafika.DrawLine(&czarny,lina_p.X,lina_p.Y,lina_k.X,lina_k.Y);
            figury[indeks].dol=660;
            figury[indeks].gora=600;

            for(int i=0;i<figury.size();i++)
            {
                if(figury[i].typ=='K')
                {
                    grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
                }
                else if(figury[i].typ=='O')
                {
                    grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
                }
                else if(figury[i].typ=='T')
                {
                    Point point1(figury[i].lewy,figury[i].dol);
                    Point point2(figury[i].prawy,figury[i].dol);
                    Point point3(figury[i].lewy+30,figury[i].gora);
                    Point points[3] = {point1, point2, point3};
                    Point* pPoints=points;
                    grafika.DrawPolygon(&czarny,pPoints,3);
                }
            }
            for(int i=0;i<wieza.size();i++)
                grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

            wieza.push_back(figury[indeks]);
            figury[indeks].dol=660;
            figury[indeks].lewy=670;
            figury[indeks].gora=600;
            figury[indeks].prawy=730;
            indeks=-1;
            wieza_wys++;
            ladunek=false;
            EndPaint(hwnd,&ps);
        }
        //jesli wieza ma wysokosc od 1 do 2
        else if(wieza_wys>0 and wieza_wys<3 and figury[indeks].lewy+30<wieza[wieza_wys-1].prawy and figury[indeks].lewy+30>wieza[wieza_wys-1].lewy
        and  figury[indeks].dol+5>=wieza[wieza_wys-1].gora)
        {
            InvalidateRect(hwnd,&obszar,TRUE);
            hdc=BeginPaint(hwnd,&ps);

            Graphics grafika(hdc);
            Pen czarny(Color(255,0,0,0), 3);

            grafika.DrawLine(&czarny,lina_p.X,lina_p.Y,lina_k.X,lina_k.Y);
            figury[indeks].dol=wieza[wieza_wys-1].gora;
            figury[indeks].gora=figury[indeks].dol-60;
            figury[indeks].lewy=wieza[wieza_wys-1].lewy;
            figury[indeks].prawy=wieza[wieza_wys-1].prawy;

            for(int i=0;i<figury.size();i++)
            {
                if(figury[i].typ=='K')
                {
                    grafika.DrawRectangle(&czarny,figury[i].lewy,figury[i].gora,60,60);
                }
                else if(figury[i].typ=='O')
                {
                    grafika.DrawEllipse(&czarny,figury[i].lewy,figury[i].gora,60,60);
                }
                else if(figury[i].typ=='T')
                {
                    Point point1(figury[i].lewy,figury[i].dol);
                    Point point2(figury[i].prawy,figury[i].dol);
                    Point point3(figury[i].lewy+30,figury[i].gora);
                    Point points[3] = {point1, point2, point3};
                    Point* pPoints=points;
                    grafika.DrawPolygon(&czarny,pPoints,3);
                }
            }
            for(int i=0;i<wieza.size();i++)
                grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

            wieza.push_back(figury[indeks]);
            wieza_wys++;
            if(wieza_wys<3)
            {
                figury[indeks].dol=660;
                figury[indeks].lewy=670;
                figury[indeks].gora=600;
                figury[indeks].prawy=730;
            }
            else
                figury.erase(figury.begin()+indeks);

            indeks=-1;
            ladunek=false;
            EndPaint(hwnd,&ps);
        }
    }
    else        //jesli nie ma zaczepionego ladunku
    {
        for(int i=0;i<figury.size();i++)
        {
            //jesli lina jest nad kolem
            if(figury[i].typ=='O' and lina_k.X>figury[i].lewy and lina_k.X<figury[i].prawy and lina_k.Y>=figury[i].gora)
            {
                indeks=i;
                InvalidateRect(hwnd,&obszar,TRUE);
                hdc=BeginPaint(hwnd,&ps);

                Graphics grafika(hdc);
                Pen czarny(Color(255,0,0,0), 3);

                grafika.DrawLine(&czarny,lina_p.X,lina_p.Y,lina_k.X,figury[figury.size()-1].gora);
                lina_k.Y=figury[figury.size()-1].gora;
                figury[figury.size()-1].lewy=lina_k.X-30;
                figury[figury.size()-1].prawy=figury[figury.size()-1].lewy+60;
                for(int j=0;j<figury.size();j++)
                {
                    if(figury[j].typ=='K')
                    {
                        grafika.DrawRectangle(&czarny,figury[j].lewy,figury[j].gora,60,60);
                    }
                    else if(figury[j].typ=='O')
                    {
                        grafika.DrawEllipse(&czarny,figury[j].lewy,figury[j].gora,60,60);
                    }
                    else if(figury[j].typ=='T')
                    {
                        Point point1(figury[j].lewy,figury[j].dol);
                        Point point2(figury[j].prawy,figury[j].dol);
                        Point point3(figury[j].lewy+30,figury[j].gora);
                        Point points[3] = {point1, point2, point3};
                        Point* pPoints=points;
                        grafika.DrawPolygon(&czarny,pPoints,3);
                    }
                }
                for(int i=0;i<wieza.size();i++)
                    grafika.DrawEllipse(&czarny,wieza[i].lewy,wieza[i].gora,60,60);

                EndPaint(hwnd,&ps);
                ladunek=true;
            }
        }
    }
}
