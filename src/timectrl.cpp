    /////////////////////////////////////////////////////////////////////////////
    // Name:        timectrl.cpp
    // Purpose:     TimePicker class
    // Author:      Christopher Hodges
    // Modified by:
    // Created:     6/15/2005
    // Copyrigh (c) Christopher Hodges
    /////////////////////////////////////////////////////////////////////////////
    #include"timectrl.h"
   // #include"utils.hpp"



    enum{
         hourText = 1000,
         minuteText = 1001,
         amPmText = 1002,
         spinner = 1003,
         text = 1004
         };


    BEGIN_EVENT_TABLE(wxTimeTextCtrl, wxTextCtrl)
      EVT_CHAR(wxTimeTextCtrl::OnKeyPress)
    END_EVENT_TABLE()

    wxTimeTextCtrl::wxTimeTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value,
             const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
       : wxTextCtrl(parent,id,value,pos,size,style,validator,name)
    {}
	

    void wxTimeTextCtrl::OnKeyPress(wxKeyEvent& event)
    {
         int keyCode = event.GetKeyCode();
		 //wxMessageBox(wxT("here"));
         switch(keyCode)
         {
            case WXK_RIGHT:
                 ArrowRight();
                 break;
            case WXK_LEFT:
                 ArrowLeft();
                 break;
            case WXK_UP:
                 OnArrowUp();
                 break;
            case WXK_DOWN:
                 OnArrowDown();
                 break;
            default:
                 break;
             //    event.Skip();
         }
    }
    // Here is the format of the time:
    //       12 : 00   pm
    //       01 2 34 5 67
    void wxTimeTextCtrl::OnArrowUp()
    {
         //First lets determine where we are
         int x = GetInsertionPoint();
         wxString fullval = GetValue();
         wxString val = GetValue().Mid(x,2);
         int cvt;

         if(x == 0){
			 double d;
			 val.ToDouble(&d);
			 cvt = d;
               if(cvt >= 23){
                      cvt=0;
               }else{
                      cvt++;
               }			  
               SetValue(wxString::Format(wxT("%2d%s"),cvt,GetValue().Mid(2)));
               SetInsertionPoint(x);
         }else if(x == 3){
			   
			   double d;

               val.ToDouble(&d);
			   cvt = d;
               if((cvt+5) >= 60){
                      cvt=0;
               }else{
                      cvt+=5;
               }
               SetValue(wxString::Format(wxT("%s%.2d%s"),GetValue().Mid(0,3).c_str(),cvt,GetValue().Mid(5).c_str()));
               SetInsertionPoint(x);
         }else if(x == 6){
               (val.IsSameAs(wxT("am"))) ? SetValue( wxString::Format(wxT("%s %s"),GetValue().Mid(0,5).c_str(),"pm"))
                                    : SetValue( wxString::Format(wxT("%s %s"),GetValue().Mid(0,5).c_str(),"am"));
               SetInsertionPoint(x);
         }
    }
    void wxTimeTextCtrl::OnArrowDown()
    {
         int x = GetInsertionPoint();
         wxString fullval = GetValue();
         wxString val = GetValue().Mid(x,2);
         int cvt;

         if(x == 0){
               double d;

               val.ToDouble(&d);
			   cvt = d;
               if(cvt <= 0){
                      cvt=23;
               }else{
                      cvt--;
               }
               SetValue(wxString::Format(wxT("%2d%s"),cvt,GetValue().Mid(2).c_str()));
               SetInsertionPoint(x);
         }else if(x == 3){
               double d;

               val.ToDouble(&d);
			   cvt = d;
               if((cvt-5) < 0){
                      cvt=55;
               }else{
                      cvt-=5;
               }
               SetValue(wxString::Format(wxT("%s%.2d%s"),GetValue().Mid(0,3).c_str(),cvt,GetValue().Mid(5).c_str()));
               SetInsertionPoint(x);
         }else if(x == 6){
               (val.IsSameAs(wxT("am"))) ? SetValue( wxString::Format(wxT("%s %s"),GetValue().Mid(0,5).c_str(),wxT("pm")))
                                    : SetValue( wxString::Format(wxT("%s %s"),GetValue().Mid(0,5).c_str(),wxT("am")));
               SetInsertionPoint(x);
         }
    }
    void wxTimeTextCtrl::ArrowLeft()
    {
         int x = GetInsertionPoint();

         if(x==3){
           SetInsertionPoint(0);
         }else if(x==6){
           SetInsertionPoint(3);
         }
    }
    void wxTimeTextCtrl::ArrowRight()
    {
         int x = GetInsertionPoint();

         if(x==0){
           SetInsertionPoint(3);
         }else if(x==3){
           SetInsertionPoint(6);
         }
    }
