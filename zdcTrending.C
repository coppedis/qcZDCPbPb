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
#include <TGraph.h>
#include <TGaxis.h>
#include <TLegend.h>
#include <TLatex.h>

#endif

void myOptions(Int_t l);

Double_t fitfun(Double_t *x, Double_t *par);
Double_t fitfunp(Double_t *x, Double_t *par);

void zdcTrending()
{

  // *********************************************************************************
  // IMPORTANT!!!! THIS NUMBER SHOULD CORRESPOND TO THE NUMBER OF RUNS 2 BE MONITORED
  //         such number is equal to the entries in file outputQC.txt  
  //      (provided that there are no double entries -> CHECK THE RUN NO.!)
  // *********************************************************************************
  const int nRun = 4;
  // **************************************************************************

  // --------------------
  myOptions(0);
  gROOT->ForceStyle();
  // --------------------

  int run[nRun] = {0, };
  float mzna[nRun] = {0., };
  float mznc[nRun] = {0., };
  float mzpa[nRun] = {0., };
  float mzpc[nRun] = {0., };  
  float mzem1[nRun] = {0., };
  float mzem2[nRun] = {0., };
    // ----------------------------------------------------------
  float mu1na[nRun] = {0., };
  float mu1nc[nRun] = {0., };
  float mu1pa[nRun] = {0., };
  float mu1pc[nRun] = {0., };
  // ---------------------------------------------------------------------------------------------------
  float msum[nRun] = {0., };
  float mdiff[nRun] = {0., };
  // -----------------------------------------------------------------------------------------
  float xnza[nRun] = {0., };
  float ynza[nRun] = {0., };
  float xnzc[nRun] = {0., };
  float ynzc[nRun] = {0., };

  FILE* file2read = fopen("outputQC.txt","r");
  if(file2read == NULL) {
    printf("\n CAUTION! The file you tried to open does not exist!!!!\n\n");
    return;
  }
  //
  float yedgesZN[2] = {0., 0.};
  float yedgesZP[2] = {0., 0.};
  float yedges1npeak[2] = {0., 0.};
  float yedges1ppeak[2] = {0., 0.};
  //
  int i=0;
  while(!feof(file2read)){
	  int stat = fscanf(file2read, "%d  %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n ",
        &run[i], 
        &mzna[i], &mznc[i], &mzpa[i], &mzpc[i], &mzem1[i], &mzem2[i], 
        &mu1na[i], &mu1nc[i], &mu1pa[i], &mu1pc[i], 
        &msum[i], &mdiff[i],
        &xnza[i], &ynza[i], &xnzc[i], &ynzc[i]);
	  printf("  RUN %d  average values  ZNA %1.2f ZNC %1.2f ZPA %1.2f, ZPC %1.2f\n", 
        run[i], mzna[i], mznc[i], mzpa[i], mzpc[i]); 
    //
    if(i==0){
      yedgesZN[0] = mzna[i];
      yedgesZP[0] = mzna[i];
      yedges1npeak[0] = mu1na[i];
      yedges1ppeak[0] = mu1pa[i];
    }
    // setting lower limits for average value trending plots
    if(mzna[i] < yedgesZN[0]) yedgesZN[0] = mzna[i];
    if(mznc[i] < yedgesZN[0]) yedgesZN[0] = mznc[i];
    if(mzpa[i] < yedgesZP[0]) yedgesZP[0] = mzpa[i];
    if(mzpc[i] < yedgesZP[0]) yedgesZP[0] = mzpc[i];
    // setting upper limits for average value trending plots
    if(mzna[i] > yedgesZN[1]) yedgesZN[1] = mzna[i];
    if(mznc[i] > yedgesZN[1]) yedgesZN[1] = mznc[i];
    if(mzpa[i] > yedgesZP[1]) yedgesZP[1] = mzpa[i];
    if(mzpc[i] > yedgesZP[1]) yedgesZP[1] = mzpc[i];
    // setting lower limits for 1n peak trending plots
    if(mu1na[i] < yedges1npeak[0]) yedges1npeak[0] = mu1na[i];
    if(mu1nc[i] < yedges1npeak[0]) yedges1npeak[0] = mu1nc[i];
    if(mu1pa[i] < yedges1ppeak[0]) yedges1ppeak[0] = mu1pa[i];
    if(mu1pc[i] < yedges1ppeak[0]) yedges1ppeak[0] = mu1pc[i];
    // setting upper limits for 1ppeak trending plots
    if(mu1na[i] > yedges1npeak[1]) yedges1npeak[1] = mu1na[i];
    if(mu1nc[i] > yedges1npeak[1]) yedges1npeak[1] = mu1nc[i];
    if(mu1pa[i] > yedges1ppeak[1]) yedges1ppeak[1] = mu1pa[i];
    if(mu1pc[i] > yedges1ppeak[1]) yedges1ppeak[1] = mu1pc[i];
    //
    i++;
    if(i > nRun) {
      printf("\n ATTENTION!!! Attempt to read more lines than declared -> Please check nRun value in macro!!!!\n\n");
      return;
    }
    if(feof(file2read)) break;
  }
  if(yedgesZN[0]<0.) yedgesZN[0] = 0.;
  if(yedgesZP[0]<0.) yedgesZP[0] = 0.;
  if(yedges1npeak[0]<0.) yedges1npeak[0] = 0.;
  if(yedges1ppeak[0]<0.) yedges1ppeak[0] = 0.;

  float xx[nRun];
  for(int i=0; i<nRun; i++) xx[i] = (float) run[i];

  //-----------------------------------------------------
  TGraph *meanzna = new TGraph(nRun, xx, mzna);
  TGraph *meanznc = new TGraph(nRun, xx, mznc);
  TGraph *meanzpa = new TGraph(nRun, xx, mzpa);
  TGraph *meanzpc = new TGraph(nRun, xx, mzpc);
  TGraph *meanzem1 = new TGraph(nRun, xx, mzem1);
  TGraph *meanzem2 = new TGraph(nRun, xx, mzem2);
  TGraph *meanXzna =  new TGraph(nRun, xx, xnza);
  TGraph *meanYzna =  new TGraph(nRun, xx, ynza);
  TGraph *meanXznc =  new TGraph(nRun, xx, xnzc);
  TGraph *meanYznc =  new TGraph(nRun, xx, ynzc);
  TGraph *peakzna = new TGraph(nRun, xx, mu1na);
  TGraph *peakznc = new TGraph(nRun, xx, mu1nc);
  TGraph *peakzpa = new TGraph(nRun, xx, mu1pa);
  TGraph *peakzpc = new TGraph(nRun, xx, mu1pc);
  TGraph *meansum = new TGraph(nRun, xx, msum);  
  TGraph *meandiff = new TGraph(nRun, xx, mdiff); 

  //-----------------------------------------------------
  TCanvas *c1 = new TCanvas("c1", "ZNs ZPs", 0, 0, 1200, 1000);
  c1->Divide(1,2);
  //
  c1->cd(1); 
  gPad->SetGridy(1);
  /*TH2F *hax1 = new TH2F("hax1", "assi", nRun, run[0], run[nRun-1], 140, 140., 280);
  hax1->Draw("");
  hax1->GetXaxis()->SetNdivisions(-nRun); 
  hax1->GetXaxis()->SetNoExponent();
  hax1->GetXaxis()->SetTitleOffset(1.2);
  //hax1->GetXaxis()->SetTitle("RUN #");
  hax1->GetYaxis()->SetTitle("Average amplitude");
  //
  for(int i=0; i<nRun; i++){
    TString j;
    j.Append(Form("%1.0f", run[i]));
    hax1->GetXaxis()->ChangeLabel(i+1, 90, -1, -1, -1, -1, j.Data());
  }*/
  meanzna->SetMarkerColor(kAzure+6); 
  meanzna->SetLineColor(kAzure+6); 
  meanzna->SetMarkerStyle(20);
  meanzna->Draw("APL"); 
  meanzna->GetXaxis()->SetNoExponent();
  meanzna->GetXaxis()->SetTitleOffset(1.2);
  meanzna->GetYaxis()->SetTitle("Average amplitude");
  meanzna->GetXaxis()->SetRangeUser(run[0], run[nRun-1]);  
  meanzna->GetYaxis()->SetRangeUser(0., 40.);  
  meanznc->SetMarkerColor(kBlue+2);  
  meanznc->SetLineColor(kBlue+2);
  meanznc->SetMarkerStyle(20);
  meanznc->Draw("PLSAME"); 
  //
  TLegend *l1 = new TLegend(0.44,0.18,0.54,0.32);
  l1->SetFillColor(kWhite);
  l1->AddEntry(meanznc," ZNC " ,"P");
  l1->AddEntry(meanzna," ZNA " ,"P");
  l1->Draw("");
  //
  c1->cd(2);
  gPad->SetGridy(1);
  /*TH2F *hax2b = new TH2F("hax2b", "assi", nRun, xx[0], xx[nRun-1], 60, 160., 220);
  hax2b->Draw("");
  hax2b->GetXaxis()->SetNdivisions(-nRun); 
  hax2b->GetXaxis()->SetTitleOffset(1.2);
  //hax2b->GetXaxis()->SetTitle("RUN #");
  hax2b->GetXaxis()->SetNoExponent();
  hax2b->GetYaxis()->SetTitle("Average amplitude");
  //
  for(int i=0; i<nRun; i++){
    TString j;
    j.Append(Form("%1.0f", run[i]));
    hax2b->GetXaxis()->ChangeLabel(i+1, 90, -1, -1, -1, -1, j.Data());
  }*/
  meanzpa->SetMarkerColor(kAzure+8); 
  meanzpa->SetLineColor(kAzure+8); 
  meanzpa->SetMarkerStyle(21);
  meanzpa->Draw("APL"); 
  meanzpa->GetXaxis()->SetNoExponent();
  meanzpa->GetXaxis()->SetTitleOffset(1.2);
  meanzpa->GetYaxis()->SetTitle("Average amplitude");
  meanzpa->GetXaxis()->SetRangeUser(run[0], run[nRun-1]);  
  meanzpa->GetYaxis()->SetRangeUser(0., 20.);  
  meanzpc->SetMarkerColor(kPink-2);  
  meanzpc->SetLineColor(kPink-2);
  meanzpc->SetMarkerStyle(21);
  meanzpc->Draw("PLSAME"); 
  //
  TLegend *l3b = new TLegend(0.44,0.18,0.54,0.32);
  l3b->SetFillColor(kWhite);
  l3b->AddEntry(meanzpc," ZPC " ,"P");
  l3b->AddEntry(meanzpa," ZPA " ,"P");
  l3b->Draw("");
  //
  c1->Print("znzp-LHC23-cp1.gif");
  
  //-----------------------------------------------------
  TCanvas *c1b = new TCanvas("c1b", "ZEMs and timing (sum-diff)", 200, 0, 1200, 1000);
  //
  c1b->Divide(1,2);
  c1b->cd(1);
  //
  gPad->SetGridy(1);
  /*TH2F *hax2 = new TH2F("hax2", "assi", nRun, xx[0], xx[nRun-1], 60, -3.2, 2.8);
  hax2->Draw("");
  hax2->GetXaxis()->SetNdivisions(-nRun); 
  hax2->GetXaxis()->SetTitleOffset(1.2);
  //hax2->GetXaxis()->SetTitle("RUN #");
  hax2->GetXaxis()->SetNoExponent();
  hax2->GetYaxis()->SetTitle("ZN TDCs (ns)");
  //
  for(int i=0; i<nRun; i++){
    TString j;
    j.Append(Form("%1.0f", run[i]));
    hax2->GetXaxis()->ChangeLabel(i+1, 90, -1, -1, -1, -1, j.Data());
  }*/
  meansum->SetMarkerColor(kAzure+8); 
  meansum->SetLineColor(kAzure+8); 
  meansum->SetMarkerStyle(20);
  meansum->Draw("APL"); 
  meansum->GetXaxis()->SetNoExponent();
  meansum->GetXaxis()->SetTitleOffset(1.2);
  meansum->GetYaxis()->SetTitle("ZN TDCs");
  meansum->GetXaxis()->SetRangeUser(run[0], run[nRun-1]);  
  meansum->GetYaxis()->SetRangeUser(-4., 4.);  
  meandiff->SetMarkerColor(kBlue+4);  
  meandiff->SetLineColor(kBlue+4);
  meandiff->SetMarkerStyle(20);
  meandiff->Draw("PLSAME"); 
  //
  TLegend *l3 = new TLegend(0.36,0.19,0.57,0.37);
  l3->SetFillColor(kWhite);
  l3->AddEntry(meandiff," ZNC-ZNA " ,"P");
  l3->AddEntry(meansum," ZNC+ZNA " ,"P");
  l3->Draw("");
  //
  c1b->cd(2);
    gPad->SetGridy(1);
  /*TH2F *hax3 = new TH2F("hax3", "assi", nRun, xx[0], xx[nRun-1], 100, 200., 300);
  hax3->Draw("");
  hax3->GetXaxis()->SetNdivisions(-nRun); 
  hax3->GetXaxis()->SetNoExponent();
  hax3->GetXaxis()->SetTitleOffset(1.2);
  //hax3->GetXaxis()->SetTitle("RUN #");
  hax3->GetYaxis()->SetTitle("Average amplitude");
  //
  for(int i=0; i<nRun; i++){
    TString j;
    j.Append(Form("%1.0f", run[i]));
    hax3->GetXaxis()->ChangeLabel(i+1, 90, -1, -1, -1, -1, j.Data());
  }*/
  meanzem1->SetMarkerColor(kTeal-6); 
  meanzem1->SetLineColor(kTeal-6);
  meanzem1->SetMarkerStyle(29); meanzem1->SetMarkerSize(1.8);
  meanzem2->SetMarkerColor(kGreen-6); 
  meanzem2->SetLineColor(kGreen-6);
  meanzem2->SetMarkerStyle(30);  meanzem2->SetMarkerSize(1.8);
  meanzem1->Draw("APL"); 
  meanzem1->GetXaxis()->SetNoExponent();
  meanzem1->GetXaxis()->SetTitleOffset(1.2);
  meanzem1->GetYaxis()->SetTitle("Average amplitude");
  meanzem1->GetXaxis()->SetRangeUser(run[0], run[nRun-1]);  
  meanzem1->GetYaxis()->SetRangeUser(200., 300.);

  meanzem2->Draw("PLSAME"); 
  //
  TLegend *l4 = new TLegend(0.44,0.18,0.54,0.32);
  l4->SetFillColor(kWhite);
  l4->AddEntry(meanzem1," ZEM1 " ,"P");
  l4->AddEntry(meanzem2," ZEM2 " ,"P");
  l4->Draw("");
  //
  c1b->Print("zemtdc-LHC23-cp1.gif"); 
  
  //-----------------------------------------------------
  TCanvas *c2 = new TCanvas("c2", "ZN centroids", 400, 400, 1200, 1000);
  c2->Divide(1,2);
  //
  c2->cd(1);
    gPad->SetGridy(1);
  /*TH2F *hax4 = new TH2F("hax4", "assi", nRun, xx[0], xx[nRun-1], 100, -0.5, 0.5);
  hax4->Draw("");
  hax4->GetXaxis()->SetNdivisions(-nRun); 
  hax4->GetXaxis()->SetNoExponent();
  hax4->GetXaxis()->SetTitleOffset(1.2);
  //hax4->GetXaxis()->SetTitle("RUN #");
  hax4->GetYaxis()->SetTitle("X (cm)");
  //
  for(int i=0; i<nRun; i++){
    TString j;
    j.Append(Form("%1.0f", run[i]));
    hax4->GetXaxis()->ChangeLabel(i+1, 90, -1, -1, -1, -1, j.Data());
  }*/
  meanXzna->SetMarkerColor(kAzure+3); 
  meanXzna->SetLineColor(kAzure+3);
  meanXzna->SetMarkerStyle(21); 
  meanXznc->SetMarkerColor(kPink+6); 
  meanXznc->SetLineColor(kPink+6);
  meanXznc->SetMarkerStyle(21); 
  meanXzna->Draw("APL"); 
  meanXzna->GetXaxis()->SetNoExponent();
  meanXzna->GetXaxis()->SetTitleOffset(1.2);
  meanXzna->GetYaxis()->SetTitle("X_{ZN} (cm)");
  meanXzna->GetYaxis()->SetRangeUser(-0.5, 0.5);
  meanXzna->GetXaxis()->SetRangeUser(run[0], run[nRun-1]);  

  meanXznc->Draw("PLSAME");
  //
  TLegend *l5 = new TLegend(0.44,0.20,0.54,0.35);
  l5->SetFillColor(kWhite);
  l5->AddEntry(meanXzna," ZNA " ,"P");
  l5->AddEntry(meanXznc," ZNC " ,"P");
  l5->Draw("");
  //
  c2->cd(2);
    gPad->SetGridy(1);
  /*TH2F *hax45 = new TH2F("hax45", "assi", nRun, xx[0], xx[nRun-1], 100, -0.5, 0.1);
  hax45->Draw("");
  hax45->GetXaxis()->SetNdivisions(-nRun); 
  hax45->GetXaxis()->SetNoExponent();
  hax45->GetXaxis()->SetTitleOffset(1.2);
  //hax45->GetXaxis()->SetTitle("RUN #");
  hax45->GetYaxis()->SetTitle("Y (cm)");
  //
  for(int i=0; i<nRun; i++){
    TString j;
    j.Append(Form("%1.0f", run[i]));
    hax45->GetXaxis()->ChangeLabel(i+1, 90, -1, -1, -1, -1, j.Data());
  }*/
  meanYzna->SetMarkerColor(kAzure+3); 
  meanYzna->SetLineColor(kAzure+3);
  meanYzna->SetMarkerStyle(20); 
  meanYznc->SetMarkerColor(kPink-3); 
  meanYznc->SetLineColor(kPink-3);
  meanYznc->SetMarkerStyle(20); 
  meanYzna->Draw("APL"); 
  meanYzna->GetXaxis()->SetNoExponent();
  meanYzna->GetXaxis()->SetTitleOffset(1.2);
  meanYzna->GetYaxis()->SetTitle("Y_{ZN} (cm)");
  meanYzna->GetYaxis()->SetRangeUser(-0.5, 0.2);
  meanYzna->GetXaxis()->SetRangeUser(run[0], run[nRun-1]);  

  meanYznc->Draw("PLSAME");
  l5->Draw("");
  //
  c2->Print("centroids-LHC23-cp1.gif");
  
   //-----------------------------------------------------
  TCanvas *c3 = new TCanvas("c3", "peaks", 600, 0, 1200, 1000);
  c3->Divide(1,2);
  //
  c3->cd(1);
    gPad->SetGridy(1);
  /*TH2F *hax5 = new TH2F("hax5", "assi", nRun, xx[0], xx[nRun-1], 18, 18., 36.);
  hax5->Draw("");
  hax5->GetXaxis()->SetNdivisions(-nRun); 
  hax5->GetXaxis()->SetNoExponent();
  hax5->GetXaxis()->SetTitleOffset(1.2);
  hax5->GetXaxis()->SetTitle("RUN #");
  hax5->GetYaxis()->SetTitle("1n peak value");
  //
  for(int i=0; i<nRun; i++){
    TString j;
    j.Append(Form("%1.0f", run[i]));
    hax5->GetXaxis()->ChangeLabel(i+1, 90, -1, -1, -1, -1, j.Data());
  }*/
  peakzna->SetMarkerColor(kBlue+2); 
  peakzna->SetLineColor(kBlue+2); 
  peakzna->SetMarkerStyle(20);
  peakzna->Draw("APL"); 
  peakzna->GetXaxis()->SetNoExponent();
  peakzna->GetXaxis()->SetTitleOffset(1.2);
  peakzna->GetYaxis()->SetTitle("1n peak value");  
  peakzna->GetXaxis()->SetTitle("RUN number");
  peakzna->GetYaxis()->SetRangeUser(0., 5.);
  peakzna->GetXaxis()->SetRangeUser(run[0], run[nRun-1]);  
  peakznc->SetMarkerColor(kAzure+8); 
  peakznc->SetLineColor(kAzure+8); 
  peakznc->SetMarkerStyle(20);
  peakznc->Draw("PLSAME"); 
  //
  TLegend *l7 = new TLegend(0.43,0.21,0.56,0.40);
  l7->SetFillColor(kWhite);
  l7->AddEntry(peakznc," ZNC  " ,"P");
  l7->AddEntry(peakzna," ZNA  " ,"P");
  l7->Draw("");

  /*TLatex *t = new TLatex();
    t->SetNDC();
    t->SetTextAlign(22);
    t->SetTextFont(42);
    t->SetTextSize(0.056);

    t->DrawLatex(0.43,0.9,"ALICE Performance");
    t->SetTextSize(0.056);
   t->DrawLatex(0.43,0.75,"Pb-Pb #sqrt{#it{s_{NN}}} = 5.36 TeV");*/
  //
  c3->cd(2);
    gPad->SetGridy(1);
  /*TH2F *hax6 = new TH2F("hax6", "assi", nRun, xx[0], xx[nRun-1], 60, 40., 100.);
  hax6->Draw("");
  hax6->GetXaxis()->SetNdivisions(-nRun);  
  hax6->GetXaxis()->SetNoExponent();
  hax6->GetXaxis()->SetTitleOffset(1.2);
  //hax6->GetXaxis()->SetTitle("RUN #");
  hax6->GetYaxis()->SetTitle("1p peak value");
  //
  for(int i=0; i<nRun; i++){
    TString j;
    j.Append(Form("%1.0f", run[i]));
    hax6->GetXaxis()->ChangeLabel(i+1, 90., -1, -1, -1, -1, j.Data());
  }*/
  peakzpa->SetMarkerColor(kAzure+7); 
  peakzpa->SetLineColor(kAzure+7); 
  peakzpa->SetMarkerStyle(21);
  peakzpa->Draw("APL"); 
  peakzpa->GetXaxis()->SetNoExponent();
  peakzpa->GetXaxis()->SetTitleOffset(1.2);
  peakzpa->GetYaxis()->SetTitle("1p peak value");
  peakzpa->GetXaxis()->SetTitle("RUN number");
  peakzpa->GetYaxis()->SetRangeUser(0., 5.);
  peakzpa->GetXaxis()->SetRangeUser(run[0], run[nRun-1]);  
  //
  peakzpc->SetMarkerColor(kPink-3); 
  peakzpc->SetLineColor(kPink-3); 
  peakzpc->SetMarkerStyle(21);
  peakzpc->Draw("PLSAME"); 
  //
  TLegend *l8 = new TLegend(0.43,0.21,0.56,0.40);
  l8->SetFillColor(kWhite);
  l8->AddEntry(peakzpc," ZPC " ,"P");
  l8->AddEntry(peakzpa," ZPA " ,"P");
  l8->Draw("");
  //
  c3->Print("peaktrend-LHC23-cp1.gif");

  

}

 void myOptions(Int_t lStat=0)
 {
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
   gStyle->SetLabelSize(0.034,"xyz");
   gStyle->SetLabelFont(font,"xyz");
   gStyle->SetLabelOffset(0.04,"xyz");
   gStyle->SetTitleFont(font,"xyz");
   gStyle->SetTitleOffset(0.9,"xyz");
   gStyle->SetTitleSize(0.06,"xyz");
   gStyle->SetMarkerSize(1.5);
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
   gStyle->SetPadLeftMargin(0.1);
   gStyle->SetPadTopMargin(0.04);
   gStyle->SetPadRightMargin(0.04);
   gStyle->SetPadBottomMargin(0.15);

 }
