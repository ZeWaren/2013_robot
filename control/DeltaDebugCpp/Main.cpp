//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include "pccontrol.h"
#include <winbase.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPort"
#pragma link "CPortCtl"
#pragma resource "*.dfm"
TFMain *FMain;
//---------------------------------------------------------------------------
__fastcall TFMain::TFMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFMain::Button1Click(TObject *Sender)
{
  ComPort1->ShowSetupDialog();
  ComPort1->Open();
  ComPort1->ClearBuffer(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Button2Click(TObject *Sender)
{
  ComPort1->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Button4Click(TObject *Sender)
{
	pccontrol_frompc data;
	data.command = PCCONTROL_GETQEIS;
	layer2p_send(&l2ctx, (unsigned char *)&data, sizeof(data));
	void* ComHandle=(void*)ComPort1->Handle;
	FlushFileBuffers(ComHandle);
	//PurgeComm(ComHandle,PURGE_RXCLEAR);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ComPort1RxChar(TObject *Sender, int Count)
{
  unsigned char i;
  unsigned char d;
  for(i=Count; i--; ) {
	  ComPort1->Read(&d, 1);
      layer2p_receive(&l2ctx, d);
  }
}
//---------------------------------------------------------------------------

void l2_linksend(unsigned char data, unsigned char lastbyte) {
  FMain->ComPort1->Write(&data, 1);
}
void l2_received(unsigned char *data, unsigned int size) {
  pccontrol_to_pc *rdata = (pccontrol_to_pc *)data;
//  ShowMessage(UIntToStr((unsigned int)rdata->command));
  switch(rdata->command) {
	  case PCCONTROL_GETQEIS:
		FMain->tbQEI1->Position = rdata->data.data_i[0];
		FMain->tbQEI2->Position = rdata->data.data_i[1];
		FMain->tbQEI3->Position = rdata->data.data_i[2];
		FMain->tbQEI4->Position = rdata->data.data_i[3];
		FMain->tbQEI5->Position = rdata->data.data_i[4];
		FMain->tbQEI6->Position = rdata->data.data_i[5];
	  break;
	  case PCCONTROL_GETKPDIS:
		FMain->leKp1->Text = FloatToStr(rdata->data.data_f[0]);
		FMain->leKp2->Text = FloatToStr(rdata->data.data_f[1]);
		FMain->leKp3->Text = FloatToStr(rdata->data.data_f[2]);
		FMain->leKp4->Text = FloatToStr(rdata->data.data_f[3]);
		FMain->leKp5->Text = FloatToStr(rdata->data.data_f[4]);
		FMain->leKp6->Text = FloatToStr(rdata->data.data_f[5]);

		FMain->leKd1->Text = FloatToStr(rdata->data.data_f[6]);
		FMain->leKd2->Text = FloatToStr(rdata->data.data_f[7]);
		FMain->leKd3->Text = FloatToStr(rdata->data.data_f[8]);
		FMain->leKd4->Text = FloatToStr(rdata->data.data_f[9]);
		FMain->leKd5->Text = FloatToStr(rdata->data.data_f[10]);
		FMain->leKd6->Text = FloatToStr(rdata->data.data_f[11]);

		FMain->leKi1->Text = FloatToStr(rdata->data.data_f[12]);
		FMain->leKi2->Text = FloatToStr(rdata->data.data_f[13]);
		FMain->leKi3->Text = FloatToStr(rdata->data.data_f[14]);
		FMain->leKi4->Text = FloatToStr(rdata->data.data_f[15]);
		FMain->leKi5->Text = FloatToStr(rdata->data.data_f[16]);
		FMain->leKi6->Text = FloatToStr(rdata->data.data_f[17]);
	  break;
  }
}
void l2_errored(void) {
  ShowMessage("Err!");
}


void __fastcall TFMain::FormCreate(TObject *Sender)
{
  layer2p_ctx_init(&l2ctx);
  layer2p_set_linksend_callback(&l2ctx, l2_linksend);
  layer2p_set_callback(&l2ctx, l2_received);
  layer2p_set_error_callback(&l2ctx, l2_errored);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Button5Click(TObject *Sender)
{
	pccontrol_frompc data;
	data.command = PCCONTROL_SETGOALS;
	data.data.data_i[0] = tbGoal1->Position;
	data.data.data_i[1] = tbGoal2->Position;
	data.data.data_i[2] = tbGoal3->Position;
	data.data.data_i[3] = tbGoal4->Position;
	data.data.data_i[4] = tbGoal5->Position;
	data.data.data_i[5] = tbGoal6->Position;
	layer2p_send(&l2ctx, (unsigned char *)&data, sizeof(data));
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Button6Click(TObject *Sender)
{
	pccontrol_frompc data;
	data.command = PCCONTROL_GETKPDIS;
	layer2p_send(&l2ctx, (unsigned char *)&data, sizeof(data));
}
//---------------------------------------------------------------------------


void __fastcall TFMain::SetQEIS(TObject *Sender)
{
	pccontrol_frompc data;
	data.command = PCCONTROL_SETQEIS;
	data.data.data_i[0] = tbQEISet1->Position;
	data.data.data_i[1] = tbQEISet2->Position;
	data.data.data_i[2] = tbQEISet3->Position;
	data.data.data_i[3] = tbQEISet4->Position;
	data.data.data_i[4] = tbQEISet5->Position;
	data.data.data_i[5] = tbQEISet6->Position;
	layer2p_send(&l2ctx, (unsigned char *)&data, sizeof(data));
}
//---------------------------------------------------------------------------

void __fastcall TFMain::Button9Click(TObject *Sender)
{
	void* ComHandle=(void*)ComPort1->Handle;
	FlushFileBuffers(ComHandle);
}
//---------------------------------------------------------------------------

void __fastcall TFMain::ComPort1RxFlag(TObject *Sender)
{
ShowMessage("Flag");
}
//---------------------------------------------------------------------------



void __fastcall TFMain::Button3Click(TObject *Sender)
{
	pccontrol_frompc data;
	data.command = PCCONTROL_SETKPDIS;
	data.data.data_f[0] = StrToFloatDef(leKp1->Text, 0);
	data.data.data_f[1] = StrToFloatDef(leKp2->Text, 0);
	data.data.data_f[2] = StrToFloatDef(leKp3->Text, 0);
	data.data.data_f[3] = StrToFloatDef(leKp4->Text, 0);
	data.data.data_f[4] = StrToFloatDef(leKp5->Text, 0);
	data.data.data_f[5] = StrToFloatDef(leKp6->Text, 0);
	data.data.data_f[6] = StrToFloatDef(leKd1->Text, 0);
	data.data.data_f[7] = StrToFloatDef(leKd2->Text, 0);
	data.data.data_f[8] = StrToFloatDef(leKd3->Text, 0);
	data.data.data_f[9] = StrToFloatDef(leKd4->Text, 0);
	data.data.data_f[10] = StrToFloatDef(leKd5->Text, 0);
	data.data.data_f[11] = StrToFloatDef(leKd6->Text, 0);
	data.data.data_f[12] = StrToFloatDef(leKi1->Text, 0);
	data.data.data_f[13] = StrToFloatDef(leKi2->Text, 0);
	data.data.data_f[14] = StrToFloatDef(leKi3->Text, 0);
	data.data.data_f[15] = StrToFloatDef(leKi4->Text, 0);
	data.data.data_f[16] = StrToFloatDef(leKi5->Text, 0);
	data.data.data_f[17] = StrToFloatDef(leKi6->Text, 0);
	layer2p_send(&l2ctx, (unsigned char *)&data, sizeof(data));
}
//---------------------------------------------------------------------------

