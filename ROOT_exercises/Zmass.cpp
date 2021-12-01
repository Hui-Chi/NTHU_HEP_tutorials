#include <iostream>
#include <cmath>
#include <string>

#include "Rtypes.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"


void Zmass() {

    //Load the file
    TFile f("~phsu/public/RootExercise/mc11_7TeV.107650.AlpgenJimmyZeeNp0_pt20.merge.NTUP_SMWZ_000551.root");

    //Get the Tree
    TTree *t = (TTree*)f.Get("physics");

    //Select branches
    t->SetBranchStatus("*",0); //inactivate all branches first, and then activate the needed branches only

    t->SetBranchStatus("el_n",1); //number of electrons
    t->SetBranchStatus("el_pt",1); //electron pt
    t->SetBranchStatus("el_eta",1); //electron eta
    t->SetBranchStatus("el_phi",1); //electron phi
    t->SetBranchStatus("el_m",1); //electron mass
    t->SetBranchStatus("el_charge",1); //electron charge

    //Define the variable to store the physical quantities
    Int_t el_n;
    vector<float> *el_pt = new vector<float>();
    vector<float> *el_eta = new vector<float>();
    vector<float> *el_phi = new vector<float>();
    vector<float> *el_m = new vector<float>();
    vector<float> *el_charge = new vector<float>();

    //Match the branches to the defined variables
    t->SetBranchAddress("el_n",&el_n);
    t->SetBranchAddress("el_pt",&el_pt);
    t->SetBranchAddress("el_eta",&el_eta);
    t->SetBranchAddress("el_phi",&el_phi);
    t->SetBranchAddress("el_m",&el_m);
    t->SetBranchAddress("el_charge",&el_charge);
    
    //Define histograms to store Zmass
    TH1F *hist_Zmass = new TH1F("mZ", "m_{Z}", 25, 66., 116.);

    //Get number of entries
    Int_t Nentries = (Int_t)t->GetEntries();

    //Loop over each event to construct Z mass
    for (Int_t i=0; i<Nentries; i++){
        t->GetEntry(i);

        /*
        1. Select events with 2 electrons with differnt charges
        2. Required the electrons to be pT>25 GeV
        3. Required the electrons to be |el_eta|<2.5
        */
        if ( el_n==2 && (*el_charge)[0]*(*el_charge)[1]<0 ){
            if ( (*el_pt)[0]>25000 && (*el_pt)[1]>25000 ){
                if ( fabs((*el_eta)[0])<2.5 && fabs((*el_eta)[1])<2.5 ) {

                    TLorentzVector e1, e2;
                    e1.SetPtEtaPhiM((*el_pt)[0], (*el_eta)[0], (*el_phi)[0], (*el_m)[0]);
                    e2.SetPtEtaPhiM((*el_pt)[1], (*el_eta)[1], (*el_phi)[1], (*el_m)[1]);

                    TLorentzVector Z;
                    Z = e1 + e2;

                    hist_Zmass->Fill(Z.M()/1000.);
                }
            }
        }
    }

    //Plot the histogram
    TCanvas *cv1 = new TCanvas();

    hist_Zmass->SetLineColor(1);
    hist_Zmass->SetLineWidth(2);
    hist_Zmass->SetYTitle("# of events");
    hist_Zmass->SetXTitle("m_{Z} [GeV]");
    hist_Zmass->Draw("hist");

    cv1->SaveAs("Zmass.pdf");

}
