//---------------------------------------------------------------------------

#ifndef FormServerH
#define FormServerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TfmServer : public TForm
{
__published:	// IDE-managed Components
        TServerSocket *ServerSocket;
        TLabel *lbOp;
        void __fastcall ServerSocketGetSocket(TObject *Sender, int Socket,
          TServerClientWinSocket *&ClientSocket);
private:	// User declarations
public:		// User declarations
        __fastcall TfmServer(TComponent* Owner);
};

const int BUF_SIZE=1024;

class TMyClientSocket : public TServerClientWinSocket
{
   char buf[BUF_SIZE];
   int off;
   void __fastcall OnError (System::TObject* Sender, TCustomWinSocket* Socket, TErrorEvent ErrorEvent, int &ErrorCode);
   void __fastcall OnEvent (System::TObject* Sender, TCustomWinSocket* Socket, TSocketEvent SocketEvent);
  public:
   TMyClientSocket(int Socket, TServerWinSocket* ServerWinSocket)
     : TServerClientWinSocket(Socket, ServerWinSocket)
   { OnErrorEvent=OnError;
     OnSocketEvent=OnEvent;
     off=0;
   }
};
//---------------------------------------------------------------------------
extern PACKAGE TfmServer *fmServer;
//---------------------------------------------------------------------------
#endif
