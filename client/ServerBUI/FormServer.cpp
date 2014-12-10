//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "FormServer.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmServer *fmServer;


//---------------------------------------------------------------------------
__fastcall TfmServer::TfmServer(TComponent* Owner)
        : TForm(Owner)
{   ServerSocket->Port=4000;
    ServerSocket->Open();
}

void __fastcall TfmServer::ServerSocketGetSocket(TObject *Sender,
      int Socket, TServerClientWinSocket *&ClientSocket)
{  ClientSocket = new TMyClientSocket(Socket, ServerSocket->Socket);
}

//---------------------------------------------------------------------------
void __fastcall TMyClientSocket::OnError
(System::TObject* Sender, TCustomWinSocket* Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{   ErrorCode=0;
}

void __fastcall TMyClientSocket::OnEvent
(System::TObject* Sender, TCustomWinSocket* Socket, TSocketEvent SocketEvent)
{ if (SocketEvent==seRead)
  { int n=ReceiveBuf(buf, BUF_SIZE-off);
    struct
    { double a;
      double b;
    } data;
    if (off+n>=sizeof(data))
    { memcpy(&data, buf, sizeof(data));
      off=0;
      double c=data.a+data.b;
      AnsiString op;
      op.printf("%.2f+%.2f=%.2f", data.a, data.b, c);
      fmServer->lbOp->Caption=op;
      SendBuf(&c, sizeof(double));
    }
  }
}
