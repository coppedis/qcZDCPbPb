#if !defined(__CINT__) || defined(__MAKECINT__)

#include <stdio.h>
#include <stdlib.h>
#include <TROOT.h>
#include <Riostream.h>
#include <TClassTable.h>
#include <TStyle.h>
#include <TMath.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TF1.h>
#include <TH1.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TLine.h>
#include <TList.h>
#include <TLatex.h>
#include <TLegend.h>
#include "QualityControl/MonitorObject.h"
#include "QualityControl/MonitorObjectCollection.h"
#include "QualityControl/QualityObject.h"
#include "QualityControl/ObjectsManager.h"

#endif

//namespace o2::quality_control::core{
void myOptions(Int_t l);

Double_t fitfun(Double_t *x, Double_t *par);
Double_t fitfunp(Double_t *x, Double_t *par);

void qcAnalysis(int nRun=0)
{
  // ***************************************************************************
  // IMPORTANT: ONE SHOULD SET HERE WHETHER ZDC IS CALIBRATED IN ENERGY OR NOT
  bool isZDCcalibrated = kFALSE;
  // ***************************************************************************
  bool printScreen = kTRUE; // set kFALSE to avoid printing values on screen
  // -----------------------
  myOptions(0);
  gROOT->ForceStyle();
  // -----------------------

 TString fn;
 if(nRun==0){
   printf(" Please, provide a run no.!!!\n\n");
   return;
 }
 else fn.Append(Form("QC_fullrun_%d.root",nRun));

 TFile *fin = TFile::Open(fn.Data(), "READ");
 printf("\n *** Opening file %s ***\n", fn.Data());
 
 o2::quality_control::core::MonitorObjectCollection *mon = (o2::quality_control::core::MonitorObjectCollection *) fin->Get("ZDC/Rec");
 //o2::quality_control::core::MonitorObjectCollection *mon = (o2::quality_control::core::MonitorObjectCollection *) fin->Get("in/ZDC/Rec");
 
 if(!mon){
   printf("ATTENTION: no valid MonitorObjectCollection found!!! Please check carefully the dir structure in root file\n\n");
   return;
 }
 else{
   printf(" MonitorObjectCollection connected\n");
 }
 TH1F *hZNA1n = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZNA_TC_A_H"))->getObject();
 TH1F *hZNC1n = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZNC_TC_A_H"))->getObject();
 TH1F *hZPA1p = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZPA_TC_A_H"))->getObject();
 TH1F *hZPC1p = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZPC_TC_A_H"))->getObject();
 TH2F *hcentrZNA = (TH2F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_CENTR_ZNA"))->getObject();
 TH2F *hcentrZNC = (TH2F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_CENTR_ZNC"))->getObject();
 TH1F *hZNA = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZNA_TC_A"))->getObject();
 TH1F *hZNC = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZNC_TC_A"))->getObject();
 TH1F *hZPA = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZPA_TC_A"))->getObject();
 TH1F *hZPC = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZPC_TC_A"))->getObject();
 TH1F *hZEM1 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZEM1_A"))->getObject();
 TH1F *hZEM2 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZEM2_A"))->getObject();
 TH1F *hZNATDC = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZNA_TC_V"))->getObject();
 TH1F *hZNCTDC = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZNC_TC_V"))->getObject();
 TH1F *hZPATDC = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZPA_TC_V"))->getObject();
 TH1F *hZPCTDC = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZPC_TC_V"))->getObject();
 TH2F *hdebu   = (TH2F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_TDC_ZNC_DIFF_ZNA_ZNC_SUM_ZNA_V"))->getObject();
 TH1F *hZNCpm1 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_ADC_ZNC_T1"))->getObject(); 
 TH1F *hZNCpm2 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_ADC_ZNC_T2"))->getObject();
 TH1F *hZNCpm3 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_ADC_ZNC_T3"))->getObject();
 TH1F *hZNCpm4 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_ADC_ZNC_T4"))->getObject();
 TH1F *hZNApm1 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_ADC_ZNA_T1"))->getObject(); 
 TH1F *hZNApm2 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_ADC_ZNA_T2"))->getObject();
 TH1F *hZNApm3 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_ADC_ZNA_T3"))->getObject();
 TH1F *hZNApm4 = (TH1F *) ((o2::quality_control::core::MonitorObject *) mon->FindObject("h_ADC_ZNA_T4"))->getObject();
 
 printf(" Histograms retrieved\n");

 TCanvas *c1 = new TCanvas("c1","Fit 1n peak ZNA", 0, 0, 600,600);
  c1->cd();
  gPad->SetLogz(1);

  hdebu->Draw("colz");
  hdebu->GetXaxis()->SetTitle("t_{ZNC}-t_{ZNA} (ns)");
  hdebu->GetXaxis()->SetRangeUser(-15., 15.);
  hdebu->GetYaxis()->SetTitle("t_{ZNC}+t_{ZNA} (ns)");

  TLatex *t = new TLatex();
    t->SetNDC();
    t->SetTextAlign(22);
    t->SetTextFont(42);
    t->SetTextSize(0.05);

   t->DrawLatex(0.1,0.9,"ALICE Performance");
   t->DrawLatex(0.1,0.75,"Pb-Pb #sqrt{#it{s_{NN}}} = 5.36 TeV");

  hZNA1n->SetLineColor(kBlue+3);
  hZNA1n->Draw("");
  hZNA1n->GetXaxis()->SetTitle("ZNA signal (a.u.)");
  hZNA1n->GetYaxis()->SetTitle("Counts");
  // ******************************************
  float npara = hZNA1n->GetMaximum();
  float xmaxa = hZNA1n->GetBinCenter(hZNA1n->GetMaximumBin());
  double xlowa = 0.2*xmaxa, xupa = 3.5*xmaxa;  
  printf(" - fitting ZNA spectrum in x range: %1.0f-%1.0f\n",xlowa, xupa);
  hZNA1n->GetXaxis()->SetRangeUser(1.,300.);
  	TF1 *funcA = new TF1("fitfun", fitfun, xlowa, xupa, 5);
		funcA->SetParameters(0.95*npara, xmaxa, 0.18*xmaxa, npara*0.16, npara*0.04);
  	funcA->SetParNames("N1A","mu1A","s1A","N2A","N3A");
	  //
    funcA->SetLineColor(kPink-2);
    funcA->SetLineWidth(1);
	  hZNA1n->Fit("fitfun","RQ");
    //
    Double_t mu1A = funcA->GetParameter(1);
  
  TString s1 = "ZNApeak1n_";
  s1.Append(Form("%d.gif", nRun));
  c1->Print(s1.Data());

 TCanvas *c2 = new TCanvas("c2","Fit 1n peak ZNC", 600, 0, 600, 600);
  c2->cd();
  //gPad->SetLogy(1);
  hZNC1n->SetLineColor(kAzure+7);
  hZNC1n->Draw("");
  hZNC1n->GetXaxis()->SetTitle("ZNA signal (a.u.)");
  hZNC1n->GetYaxis()->SetTitle("Counts");
  // ******************************************
  float npar2 = hZNC1n->GetMaximum();
  float xmaxc = hZNC1n->GetBinCenter(hZNC1n->GetMaximumBin());
  double xlowc = 0.2*xmaxc, xupc = 3.5*xmaxc;  
  printf(" - fitting ZNC spectrum in x range: %1.0f-%1.0f  \n", xlowc, xupc);
  hZNC1n->GetXaxis()->SetRangeUser(1.,300.);
  	TF1 *funcC = new TF1("fitfun", fitfun, xlowc, xupc, 5);
    funcC->SetParameters(0.95*npar2, xmaxc, 0.18*xmaxc, npar2*0.16, npar2*0.04);
  	funcC->SetParNames("N1C","mu1C","s1C","N2C","N3C");
	  //
    funcC->SetLineColor(kPink);
    funcC->SetLineWidth(1);
	  hZNC1n->Fit("fitfun","RQ");
    //
    Double_t mu1C = funcC->GetParameter(1);
  
  TString s2 = "ZNCpeak1n_";
  s2.Append(Form("%d.gif", nRun));
  c2->Print(s2.Data());

TCanvas *c3 = new TCanvas("c3","Fit 1p peak ZPA", 0, 600, 500, 500);
 c3->cd();
  gPad->SetLogy(1);
  hZPA1p->SetLineColor(kAzure-4);
  hZPA1p->Draw("");
  hZPA1p->GetXaxis()->SetRangeUser(0., 250);
  hZPA1p->GetXaxis()->SetTitle("ZPA signal (a.u.)");
  hZPA1p->GetYaxis()->SetTitle("Counts");
  //
  double xlowp = 0, xupp = 0.;
  if(isZDCcalibrated) xlowp = 3., xupp = 10.;
  else xlowp = 50., xupp = 100.;
  printf(" - fitting ZP spectra in x range: %1.0f-%1.0f  \n", xlowp, xupp);
  //
  hZPA1p->Fit("gaus","Q","", xlowp, xupp);
  TF1 *f1pa = (TF1*)hZPA1p->GetListOfFunctions()->FindObject("gaus");
  double mu1pA = f1pa->GetParameter(1);
  
  TString s3 = "ZPApeak1p_";
  s3.Append(Form("%d.gif", nRun));
  c3->Print(s3.Data());


TCanvas *c4 = new TCanvas("c4","Fit 1p peak ZPC", 600, 600, 500, 500);
 c4->cd();
  gPad->SetLogy(1);
  hZPC1p->SetLineColor(kViolet-4);
  hZPC1p->Draw("");
  hZPC1p->GetXaxis()->SetRangeUser(0., 250);
  hZPC1p->GetXaxis()->SetTitle("ZPA signal (a.u.)");
  hZPC1p->GetYaxis()->SetTitle("Counts");
  //
  hZPC1p->Fit("gaus","Q","", xlowp, xupp);
  TF1 *f1pc = (TF1*)hZPC1p->GetListOfFunctions()->FindObject("gaus");
  double mu1pC = f1pc->GetParameter(1);

 TH1D *hxZNC = hcentrZNC->ProjectionX("hxZNC");
 TH1D *hyZNC = hcentrZNC->ProjectionY("hyZNC");
 TH1D *hxZNA = hcentrZNA->ProjectionX("hxZNA");
 TH1D *hyZNA = hcentrZNA->ProjectionY("hyZNA");
 //
 TH1D *hsum = hdebu->ProjectionY("hsum");
 TH1D *hdiff = hdebu->ProjectionX("hdiff");
  
  TString s4 = "ZPCpeak1p_";
  s4.Append(Form("%d.gif", nRun));
  c4->Print(s4.Data());

TCanvas *c5 = new TCanvas("c5","ZN towers", 600, 0, 1400, 600);
c5->Divide(2,1);
 c5->cd(1);
  gPad->SetLogy(1);
  hZNApm1->SetLineColor(kAzure+3);
  hZNApm1->SetLineWidth(2);
  hZNApm1->Draw("");
  hZNApm1->GetXaxis()->SetTitle("ZNA towers");
  hZNApm2->SetLineColor(kPink);
  hZNApm2->SetLineWidth(2);
  hZNApm2->Draw("same");
  hZNApm3->SetLineColor(kTeal+2);
  hZNApm3->SetLineWidth(2);
  hZNApm3->Draw("same");
  hZNApm4->SetLineColor(kAzure+1);
  hZNApm4->SetLineWidth(2);
  hZNApm4->Draw("same");
   TLegend *leg1 = new TLegend(0.6, 0.6, 0.8, 0.8);
  leg1->AddEntry(hZNApm1, "ZNA PM1", "L");
  leg1->AddEntry(hZNApm2, "ZNA PM2", "L");
  leg1->AddEntry(hZNApm3, "ZNA PM3", "L");
  leg1->AddEntry(hZNApm4, "ZNA PM4", "L");
  leg1->Draw("");

 c5->cd(2);
  gPad->SetLogy(1);
  hZNCpm1->SetLineColor(kAzure+8);
  hZNCpm1->SetLineWidth(2);
  hZNCpm1->Draw("");
  hZNCpm1->GetXaxis()->SetTitle("ZNC towers");
  hZNCpm2->SetLineColor(kGreen+2);
  hZNCpm2->SetLineWidth(2);
  hZNCpm2->Draw("same");
  hZNCpm3->SetLineColor(kMagenta);
  hZNCpm3->SetLineWidth(2);
  hZNCpm3->Draw("same");
  hZNCpm4->SetLineColor(kBlue+2);
  hZNCpm4->SetLineWidth(2);
  hZNCpm4->Draw("same");
  //
  TLegend *leg2 = new TLegend(0.6, 0.6, 0.8, 0.8);
  leg2->AddEntry(hZNCpm1, "ZNC PM1", "L");
  leg2->AddEntry(hZNCpm2, "ZNC PM2", "L");
  leg2->AddEntry(hZNCpm3, "ZNC PM3", "L");
  leg2->AddEntry(hZNCpm4, "ZNC PM4", "L");
  leg2->Draw("");

  TString s5 = "ZNtowers_";
  s5.Append(Form("%d.gif", nRun));
  c5->Print(s5.Data());

if(printScreen){
 printf("\n **************************************************************************\n");
 printf("%d \n", nRun);
 printf(" ---------------------------------------------------------------------------\n");
 printf(" %1.2f  \t %1.2f \t %1.2f \t %1.2f \t %1.2f \t %1.2f \n", hZNA->GetMean(), hZNC->GetMean(), hZPA->GetMean(), hZPC->GetMean(), hZEM1->GetMean(), hZEM2->GetMean());
 printf(" ---------------------------------------------------------------------------\n");
 printf(" %1.2f  \t %1.2f \t %1.2f \t %1.2f \n", mu1A, mu1C, mu1pA, mu1pC);
 printf(" ---------------------------------------------------------------------------\n");
 printf(" %1.2f  \t %1.2f \n", hsum->GetMean(), hdiff->GetMean());
 printf(" ---------------------------------------------------------------------------\n");
 printf(" %1.2f  \t %1.2f \t %1.2f \t %1.2f \n", hxZNA->GetMean(), hyZNA->GetMean(), hxZNC->GetMean(), hyZNC->GetMean());
}

 FILE* ftime = fopen("outputQC.txt","a");
 fprintf(ftime," %d \t %1.2f  \t %1.2f \t %1.2f \t %1.2f \t %1.2f \t %1.2f \t %1.2f  \t %1.2f \t %1.2f \t %1.2f \t %1.2f  \t %1.2f  \t %1.2f  \t %1.2f  \t %1.2f  \t %1.2f\n", 
   nRun, 
   hZNA->GetMean(), hZNC->GetMean(), hZPA->GetMean(), hZPC->GetMean(), hZEM1->GetMean(), hZEM2->GetMean(),
   mu1A, mu1C, mu1pA, mu1pC,
   hsum->GetMean(), hdiff->GetMean(),
   hxZNA->GetMean(), hyZNA->GetMean(), hxZNC->GetMean(), hyZNC->GetMean());

}
Double_t fitfun(Double_t *x, Double_t *par)
{
  Double_t fitval=0.;

  Double_t arg1=0, arg2=0, arg3=0;
  if(par[2]!= 0){
    arg1 = (x[0] - par[1])/par[2];
    arg2 = (x[0] - 2*par[1])/(TMath::Sqrt(2)*par[2]);
    arg3 = (x[0] - 3*par[1])/(TMath::Sqrt(3)*par[2]);
  }

  Double_t g1 = par[0] * TMath::Exp(-0.5*arg1*arg1);
  Double_t g2 = par[3] * TMath::Exp(-0.5*arg2*arg2);
  Double_t g3 = par[4] * TMath::Exp(-0.5*arg3*arg3);
  fitval = g1 + g2 + g3;

  return fitval;
}

Double_t fitfunp(Double_t *x, Double_t *par)
{
  Double_t fitval=0.;

  Double_t arg0=0, arg1=0, arg2=0, arg3=0;
  Double_t sigma2=0, sigma3=0;

  if(par[2]!= 0) arg0 = (x[0] - par[1])/par[2];
  if(par[5]!= 0){
    arg1 = (x[0] - par[4])/par[5];

    sigma2 = TMath::Sqrt(2*par[5]*par[5]-par[2]*par[2]);
    arg2 = (x[0] - 2*par[4])/sigma2;

    sigma3 = TMath::Sqrt(3*par[5]*par[5]-2*par[2]*par[2]);
    arg3 = (x[0] - 3*par[4])/sigma3;
  }

  Double_t g0 = par[0] * TMath::Exp(-0.5*arg0*arg0);
  Double_t g1 = par[3] * TMath::Exp(-0.5*arg1*arg1);
  Double_t g2 = par[6] * TMath::Exp(-0.5*arg2*arg2);
  Double_t g3 = par[7] * TMath::Exp(-0.5*arg3*arg3);

  fitval = g0 + g1 + g2 + g3;

  return fitval;
}
 void myOptions(Int_t lStat=0){
   // Set gStyle
   int font = 42;
   // From plain
   gStyle->SetFrameBorderMode(0);
   gStyle->SetFrameFillColor(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetPadColor(10);
   gStyle->SetCanvasColor(10);
   gStyle->SetTitleFillColor(10);
   gStyle->SetTitleBorderSize(1);
   gStyle->SetStatColor(10);
   gStyle->SetStatBorderSize(1);
   gStyle->SetLegendBorderSize(1);
   //
   gStyle->SetDrawBorder(0);
   gStyle->SetTextFont(font);
   gStyle->SetStatFont(font);
   gStyle->SetStatFontSize(0.05);
   gStyle->SetStatX(0.97);
   gStyle->SetStatY(0.98);
   gStyle->SetStatH(0.03);
   gStyle->SetStatW(0.3);
   gStyle->SetTickLength(0.02,"y");
   gStyle->SetEndErrorSize(3);
   gStyle->SetLabelSize(0.05,"xyz");
   gStyle->SetLabelFont(font,"xyz");
   gStyle->SetLabelOffset(0.01,"xyz");
   gStyle->SetTitleFont(font,"xyz");
   gStyle->SetTitleOffset(1.1,"xyz");
   gStyle->SetTitleSize(0.06,"xyz");
   gStyle->SetMarkerSize(1);
   gStyle->SetPalette(1,0);
   if (lStat){
     gStyle->SetOptTitle(1);
     gStyle->SetOptStat(1111);
     gStyle->SetOptFit(0);
     }
   else {
     gStyle->SetOptTitle(0);
     gStyle->SetOptStat(0);
     gStyle->SetOptFit(0);
   }
   gStyle->SetPadLeftMargin(0.15);
   gStyle->SetPadTopMargin(0.04);
   gStyle->SetPadRightMargin(0.04);
   gStyle->SetPadBottomMargin(0.15);

 }
//}