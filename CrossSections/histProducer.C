#include <string>
using namespace std;


void LineParser(ifstream&, vector<string>&);						//Function to turn a multiple lines file into a vector of strings
void Parser(ifstream& file_to_parse, vector<vector<string> >& );	//Function to turn a vector of lines coming from a text in a vector
							 										//of vectors containing the fields. spreadsheet should be empty
void histProducer(string fileName, string histName, string outputFileName){

	bool verbose = false;

	ifstream inputFile(fileName);
	vector< vector<string> > spreadsheet;

	Parser(inputFile, spreadsheet);

	int points = spreadsheet.size();
	vector<float> binXLow_v;
	vector<float> binCenter_v;
	vector<float> crossSection_v;

	if(verbose) cout << "Found " << points << " points" << endl;

	for(int i = 0; i <= points; i++){
		if (i < points -1 && stof(spreadsheet.at(i).at(1)) >= stof(spreadsheet.at(i+1).at(0))){
			continue;
		}
		if (verbose) cout << "Line " << i << "/" << points <<endl;
		if (i == points){
			binXLow_v.push_back(binXLow_v.back() + 2*(binCenter_v.back() - binXLow_v.back()	));
			break;
		}
		binXLow_v.push_back(stof(spreadsheet.at(i).at(0)));
		binCenter_v.push_back(stof(spreadsheet.at(i).at(1)));
		crossSection_v.push_back(stof(spreadsheet.at(i).at(2)));
		if(verbose ) cout << binXLow_v.back() << "\t" << binCenter_v.back() << "\t" << crossSection_v.back() << endl;
	}
	points = binCenter_v.size();
	if(verbose) cout << points << " points used" << endl;
	float binXLow[points];
	float binCenter[points];
	float crossSection[points];

	for(int i = 0;i<points;i++){
		binXLow[i]=binXLow_v.at(i);
		binCenter[i]=binCenter_v.at(i);
		crossSection[i]=crossSection_v.at(i);
		if(i==points-1)binXLow[points]=binXLow_v.at(points);
	}
// && TMath::IsNaN(crossSection[i])
	TH1F *hist = new TH1F(histName.c_str(),histName.c_str(),points,binXLow);
	for(int i = 0; i < points; i++) hist->Fill(binCenter[i],crossSection[i]);

	hist->SaveAs((outputFileName+".root").c_str());
}

void LineParser(ifstream& file_to_parse, vector<string>& lines){
  if (!file_to_parse){
    printf("ERROR in LineParser > file not found.\n");
    exit(1);
  }
  while (true){
    if (file_to_parse.eof())break;
    string line;
    getline(file_to_parse,line);
    if (line[0] != '#' && line.size() > 0){
      lines.push_back(line);
    }
  }
}

void Parser(ifstream& file_to_parse,vector<vector<string> >& spreadsheet){
  vector<string> lines;
  LineParser(file_to_parse,lines);
  for (vector<string>::iterator line=lines.begin();line!=lines.end();++line){
    string field_content;
    istringstream linestream(*line);
    if (!linestream.eof()){
      spreadsheet.push_back(vector<string>());
      while(1){
				getline(linestream,field_content,' ');
				spreadsheet.back().push_back(field_content);
				if (linestream.eof())break;
      }
    }
  }
}
