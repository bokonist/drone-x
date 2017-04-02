#include "drone-evaluation.h"
#include "drone.h"
#include <GL/freeglut.h>
#include <stdlib.h>
#include <cstring>
using namespace NEAT;
Population *drone_test(int gens) 
{
    Population *pop=0;
    Genome *start_genome;
    char curword[20];
    int id;

    ostringstream *fnamebuf;
    int gen;

    int evals[NEAT::num_runs];  //Hold records for each run
    int genes[NEAT::num_runs];
    int nodes[NEAT::num_runs];
    int winnernum;
    int winnergenes;
    int winnernodes;
    //For averaging
    int totalevals=0;
    int totalgenes=0;
    int totalnodes=0;
    int expcount;
    int samples;  //For averaging

    memset (evals, 0, NEAT::num_runs * sizeof(int));
    memset (genes, 0, NEAT::num_runs * sizeof(int));
    memset (nodes, 0, NEAT::num_runs * sizeof(int));

    ifstream iFile("dronestartgenes",ios::in);

    cout<<"START DRONE TEST"<<endl;

    cout<<"Reading in the start genome"<<endl;
    //Read in the start Genome
    iFile>>curword;
    iFile>>id;
    cout<<"Reading in Genome id "<<id<<endl;
    start_genome=new Genome(id,iFile);
    iFile.close();
    cout<<NEAT::num_runs;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) 
    {
      //Spawn the Population
        cout<<"new experiment\n";
      cout<<"Spawning Population off dronestartgenes"<<endl;

      pop=new Population(start_genome,NEAT::pop_size);

      cout<<"Verifying Spawned Pop"<<endl;
      pop->verify();

      for (gen=1;gen<=gens;gen++) 
      {
        cout<<"Epoch "<<gen<<endl;	

        //This is how to make a custom filename
        fnamebuf=new ostringstream();
        (*fnamebuf)<<"gen_"<<gen<<ends;  //needs end marker

        #ifndef NO_SCREEN_OUT
        cout<<"name of fname: "<<fnamebuf->str()<<endl;
        #endif

        char temp[50];
        sprintf (temp, "gen_%d", gen);

        //Check for success
        if (drone_epoch(pop,gen,temp,winnernum,winnergenes,winnernodes)) 
        {
          //	if (drone_epoch(pop,gen,fnamebuf->str(),winnernum,winnergenes,winnernodes)) {
          //Collect Stats on end of experiment
          evals[expcount]=NEAT::pop_size*(gen-1)+winnernum;
          genes[expcount]=winnergenes;
          nodes[expcount]=winnernodes;
          //gen=gens; // this is to stop if success
        }

        //Clear output filename
        fnamebuf->clear();
        delete fnamebuf;
      }

    if (expcount<NEAT::num_runs-1) 
        delete pop;
    }

    //Average and print stats
    cout<<"Nodes: "<<endl;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) 
    {
        cout<<nodes[expcount]<<endl;
        totalnodes+=nodes[expcount];
    }

    cout<<"Genes: "<<endl;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) 
    {
        cout<<genes[expcount]<<endl;
        totalgenes+=genes[expcount];
    }

    cout<<"Evals "<<endl;
    samples=0;
    for(expcount=0;expcount<NEAT::num_runs;expcount++) 
    {
        cout<<evals[expcount]<<endl;
        if (evals[expcount]>0)
        {
          totalevals+=evals[expcount];
          samples++;
        }
    }

    cout<<"Failures: "<<(NEAT::num_runs-samples)<<" out of "<<NEAT::num_runs<<" runs"<<endl;
    cout<<"Average Nodes: "<<(samples>0 ? (double)totalnodes/samples : 0)<<endl;
    cout<<"Average Genes: "<<(samples>0 ? (double)totalgenes/samples : 0)<<endl;
    cout<<"Average Evals: "<<(samples>0 ? (double)totalevals/samples : 0)<<endl;

    return pop;

}

bool drone_evaluate(Organism *org) 
{
    cout<<"new organism\n";
    Network *net;
    double out[2]; //The two outputs
    double this_out; //The current output
    int count;
    double errorsum;

    bool success;  //Check for successful activation
    int numnodes;  /* Used to figure out how many nodes
    should be visited during activation */

    int net_depth; //The max depth of the network to be activated
    int relax; //Activates until relaxation

    //The four possible input combinations to xor
    //The first number is for biasing
    /*
      1' 0 0
      1' 0 1
      1' 1 0
      1' 1 1
    */
    /*double in[4][3]=
    {
      {1.0,0.0,0.0},
      {1.0,0.0,1.0},
      {1.0,1.0,0.0},
      {1.0,1.0,1.0}
    };*/
    double in[7]; // bias, org-x, org-y, obstacle1-x, obstacle1-y, obstacle2-x, obstacle2-y

    net=org->net;
    numnodes=((org->gnome)->nodes).size();
    net_depth=net->max_depth();

    //TEST CODE: REMOVE
    //cout<<"ACTIVATING: "<<org->gnome<<endl;
    //cout<<"DEPTH: "<<net_depth<<endl;

    //Load and activate the network on each input till drone is destroyed
    while(org->droneIsAlive) 
    {
     //   cout<<"drone is alive";
        in[0]=1.0; //bias
        in[1]=1-org->normalized_x;
        in[2]=org->normalized_y;
        in[3]=0;//defaults
        in[4]=0;
        in[5]=0;
        in[6]=0;
        if(obstacleList.size()>=1)
        {
            in[3]=1-(obstacleList.front()).norm_x;
            in[4]=(obstacleList.front()).norm_y;
        }
        if(obstacleList.size() >=2)
        {
            in[5]=1-(obstacleList.at(1)).norm_x;
            in[6]=(obstacleList.at(1)).norm_y;
        }
        net->load_sensors(in);
        //Relax net and get output
        success=net->activate();
        //use depth to ensure relaxation
        for (relax=0;relax<=net_depth;relax++) 
        {
            success=net->activate();
            //this_out=(*(net->outputs.begin()))->activation;
        }
        int i=0;
        for(auto it = net->outputs.begin(); it!= net->outputs.end(); ++it)
        {
            out[i++]=(*(it))->activation;
        }
        i=0;
        if(out[0]>=0.5)
        {
            if( !(movementY <= ( resY-( (resY/2) + (resY*0.20) ) )) )
            {
                cout<<"boundaryUP";
                if(org->movesMade > 5)
                    org->movesMade--;
                goto asd1;
            }            
            org->movesMade++;
            inputKey.push_back('U');
            
        }
asd1:   if(out[1]>= 0.5)
        {
            if( !(movementY >= -( resY/2 - (resY*9)/100 )) )
            {
                cout<<"boundaryDOWN";
                if(org->movesMade > 5)
                    org->movesMade--;
                goto asd2;
            }
            org->movesMade++;
            inputKey.push_back('D');
        }
asd2:
        mixedStepLoop();
        glutMainLoopEvent();
        org->droneIsAlive= droneAlive;
        net->flush();
    }
    org->fitness= score + (org->movesMade*10);
    
    if(score < 2 || org->movesMade == 0 )
    {
        cout<<"undesirable fitness "<<org->fitness<<endl;
        org->fitness=0.0001;
    }
    resetSimulation();
    if (success) 
    {
        /*errorsum=(fabs(out[0])+fabs(1.0-out[1])+fabs(1.0-out[2])+fabs(out[3]));
        org->fitness=pow((4.0-errorsum),2);
        org->error=errorsum;*/
        errorsum=0;
        //org->fitness=score;
        org->error=errorsum;
    }
    else 
    {
      //The network is flawed (shouldnt happen)
      errorsum=999.0;
      org->fitness=0.001;
    }

    #ifndef NO_SCREEN_OUT
    cout<<"Org "<<(org->gnome)->genome_id<<"         error: "<<errorsum<<"  ["<<out[0]<<" "<<out[1]<<"]"<<endl;
    cout<<"Org "<<(org->gnome)->genome_id<<"         fitness: "<<org->fitness<<endl;
    #endif

    //  if (errorsum<0.05) { 
    //if (errorsum<0.2) {
    if (org->fitness >= 10) 
    {
      org->winner=true;
      return true;
    }
    else 
    {
      org->winner=false;
      return false;
    }
}

int drone_epoch(Population *pop,int generation,char *filename,int &winnernum,int &winnergenes,int &winnernodes) 
{
    cout<<"new generation\n";
    vector<Organism*>::iterator curorg;
    vector<Species*>::iterator curspecies;
    //char cfilename[100];
    //strncpy( cfilename, filename.c_str(), 100 );

    //ofstream cfilename(filename.c_str());

    bool win=false;


    //Evaluate each organism on a test
    for(curorg=(pop->organisms).begin();curorg!=(pop->organisms).end();++curorg) 
    {
        if (drone_evaluate(*curorg)) 
        {
            win=true;
            winnernum=(*curorg)->gnome->genome_id;
            winnergenes=(*curorg)->gnome->extrons();
            winnernodes=((*curorg)->gnome->nodes).size();
            if (winnernodes==5) 
            {
              //You could dump out optimal genomes here if desired
              //(*curorg)->gnome->print_to_filename("drone_optimal");
              //cout<<"DUMPED OPTIMAL"<<endl;
            }
        }
    }

    //Average and max their fitnesses for dumping to file and snapshot
    for(curspecies=(pop->species).begin();curspecies!=(pop->species).end();++curspecies) 
    {

        //This experiment control routine issues commands to collect ave
        //and max fitness, as opposed to having the snapshot do it, 
        //because this allows flexibility in terms of what time
        //to observe fitnesses at

        (*curspecies)->compute_average_fitness();
        (*curspecies)->compute_max_fitness();
    }

    //Take a snapshot of the population, so that it can be
    //visualized later on
    //if ((generation%1)==0)
    //  pop->snapshot();

    //Only print to file every print_every generations
    if  (win ||    ((generation%(NEAT::print_every))==0))
        pop->print_to_file_by_species(filename);
    
    if (win) 
    {
        for(curorg=(pop->organisms).begin();curorg!=(pop->organisms).end();++curorg) 
        {
            if ((*curorg)->winner) 
            {
                cout<<"WINNER IS #"<<((*curorg)->gnome)->genome_id<<endl;
                //Prints the winner to file
                //IMPORTANT: This causes generational file output!
                print_Genome_tofile((*curorg)->gnome,"drone_winner");
            }
        }
    }

    pop->epoch(generation);
    if (win) 
      return 1;
    else
      return 0;
}
