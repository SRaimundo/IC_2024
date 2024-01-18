#include <bits/stdc++.h>
#include <fstream>

using namespace std;

#define INF 1123123123

typedef pair<double,double> pdd;
typedef tuple<int,int,int> tiii;

//dp sem considerar tempo
double solve(vector<int> &ordem, vector<vector<vector<double>>> &acomodacoes,vector<vector<vector<pdd>>> &voos, vector<pair<int,int>>&limiteDias, int quantDias, int quantCidades){
    
    //[cidade,dia] = custo
    vector<vector<double>> tab(quantCidades, vector<double>(quantDias,INF));  // origem no inicio e no fim

    for(int i=1;i<quantDias;i++){ 
        tab[0][i] = voos[0][ordem[0]][i].first;
    }

    for(int i=0;i<quantCidades-1;i++){
        for(int j=1;j<quantDias;j++){
            const auto [minDias,maxDias] = limiteDias[ordem[i]];
            int cidadeAtual = ordem[i];
            int cidadeDestino;
            if(i+1 == quantCidades-1){
                cidadeDestino = 0;
            }else{
                cidadeDestino = ordem[i+1];
            }
            for(int k=minDias;k<=maxDias;k++){
                if(j+k>quantDias) break;
                double custo = tab[i][j] + acomodacoes[cidadeAtual][j][j+k] + voos[cidadeAtual][cidadeDestino][j+k].first;
                tab[i+1][j+k] = min(tab[i+1][j+k],custo);
            }
        }
    }

    double minCusto = INF;

    for(int i=1;i<quantDias;i++){
        minCusto = min(minCusto,tab[quantCidades-1][i]);
    }

    return minCusto;

}

//imprime rota da dp sem cosiderar o tempo
void imprimeRota(vector<int> &ordem, vector<vector<vector<double>>> &acomodacoes,vector<vector<vector<pdd>>> &voos, vector<pair<int,int>>&limiteDias, int quantDias, int quantCidades, map<int,string> &idCidade){

    //[cidade,dia] = custo
    vector<vector<double>> tab(quantCidades, vector<double>(quantDias,INF));  // origem no inicio e no fim

    vector<vector<pair<int,int> >> retorno(quantCidades,vector<pair<int,int>>(quantDias));

    for(int i=1;i<quantDias;i++){ 
        tab[0][i] = voos[0][ordem[0]][i].first;
    }

    for(int i=0;i<quantCidades-1;i++){
        for(int j=1;j<quantDias;j++){
            const auto [minDias,maxDias] = limiteDias[ordem[i]];
            int cidadeAtual = ordem[i];
            int cidadeDestino;
            if(i+1 == quantCidades-1){
                cidadeDestino = 0;
            }else{
                cidadeDestino = ordem[i+1];
            }
            for(int k=minDias;k<=maxDias;k++){
                if(j+k>quantDias) break;
                double custo = tab[i][j] + voos[cidadeAtual][cidadeDestino][j+k].first + acomodacoes[cidadeAtual][j][j+k];
                if(tab[i+1][j+k] > custo){
                    tab[i+1][j+k] = custo;
                    retorno[i+1][j+k] = {i,j};
                }
                
            }
        }
    }

    double minCusto = INF;
    int idMenor;

    for(int i=1;i<quantDias;i++){
        if(minCusto > tab[quantCidades-1][i]){
            minCusto = tab[quantCidades-1][i];
            idMenor = i;
        }
    }

    int cidade=quantCidades-1, dia = idMenor;

    vector<tuple<int,int,int>> res;

    while(cidade>0){
        pair<int,int> atual = retorno[cidade][dia];
        res.push_back({atual.first,atual.second,dia});
        cidade = atual.first;
        dia = atual.second;
    }

    for(int i=res.size()-1;i>=0;i--){
        const auto[a,b,c] = res[i];
        cout << idCidade[ordem[a]] << " do dia " << b << " ao dia " << c << endl;
    }

}

//dp considerando o tempo
pdd solve2(vector<int> &ordem, vector<vector<vector<double>>> &acomodacoes,vector<vector<vector<pdd>>> &voos, vector<pair<int,int>>&limiteDias, int quantDias, int quantCidades,int limiteTempo){

    //[cidade,dia,tempo]
    vector<vector<vector<double>>> tab(quantCidades, vector<vector<double>>(quantDias, vector<double>(limiteTempo+1, INF)));


    for(int i=1;i<quantDias;i++){
        if(voos[0][ordem[0]][i].second > limiteTempo) continue;
        tab[0][i][voos[0][ordem[0]][i].second] = voos[0][ordem[0]][i].first;
    }


    for(int i=1;i<quantCidades;i++){
        for(int j=1;j<quantDias;j++){
            const auto [minDias,maxDias] = limiteDias[ordem[i-1]];
            int anterior = ordem[i-1];
            int atual = i==quantCidades-1? 0 : ordem[i];
            
            for(int k = minDias;k<=maxDias;k++){
                if(j-k<=0) break;
                for(int t = 0; t <=limiteTempo;t++){
                    if(t >= voos[anterior][atual][j].second){
                        double custo = tab[i-1][j-k][t-voos[anterior][atual][j].second] + acomodacoes[anterior][j-k][j] + voos[anterior][atual][j].first;
                        tab[i][j][t] = min(tab[i][j][t],custo);
                    }

                }
            }

        }
    }


    double minCusto = INF;
    double tempo = INF;

    for(int i=1;i<quantDias;i++){
        for(int t = 0; t <=limiteTempo;t++){
            if(minCusto >tab[quantCidades-1][i][t]){
                minCusto = tab[quantCidades-1][i][t];
                tempo = t;
            }
        }
    }

    return {minCusto,tempo};   



}


//imprime a rota da dp considerando o tempo
void imprimeRota2(vector<int> &ordem, vector<vector<vector<double>>> &acomodacoes,vector<vector<vector<pdd>>> &voos, vector<pair<int,int>>&limiteDias, int quantDias, int quantCidades,int limiteTempo, map<int,string> &idCidade){

    //[cidade,dia,tempo]
    vector<vector<vector<double>>> tab(quantCidades, vector<vector<double>>(quantDias, vector<double>(limiteTempo+1, INF)));


    vector<vector<vector<tiii> > > retorno(quantCidades,vector<vector<tiii> > (quantDias,vector<tiii>(limiteTempo+1)));


    for(int i=1;i<quantDias;i++){
        if(voos[0][ordem[0]][i].second > limiteTempo) continue;
        tab[0][i][voos[0][ordem[0]][i].second] = voos[0][ordem[0]][i].first;
    }


    for(int i=1;i<quantCidades;i++){
        for(int j=1;j<quantDias;j++){
            const auto [minDias,maxDias] = limiteDias[ordem[i-1]];
            int anterior = ordem[i-1];
            int atual = i==quantCidades-1? 0 : ordem[i];
            
            for(int k = minDias;k<=maxDias;k++){
                if(j-k<=0) break;
                for(int t = 0; t <=limiteTempo;t++){
                    if(t >= voos[anterior][atual][j].second){
                        double custo = tab[i-1][j-k][t-voos[anterior][atual][j].second] + acomodacoes[anterior][j-k][j] + voos[anterior][atual][j].first;

                        if(tab[i][j][t] > custo){
                            tab[i][j][t] = custo;
                            retorno[i][j][t] = {i-1,j-k,t-voos[anterior][atual][j].second};
                        }
                    }

                }
            }

        }
    }


    double minCusto = INF;
    double tempo = INF;

    int idMenor;

    for(int i=1;i<quantDias;i++){
        for(int t = 0; t <=limiteTempo;t++){
            if(minCusto >tab[quantCidades-1][i][t]){
                minCusto = tab[quantCidades-1][i][t];
                tempo = t;
                idMenor = i;
            }
        }
    }

    int cidade=quantCidades-1, dia = idMenor;

    vector<tuple<int,int,int>> res;

    while(cidade>0){
        tiii atual = retorno[cidade][dia][tempo];
        auto [a,b,c] = atual;
        res.push_back({a,b,dia});
        cidade = a;
        dia = b;
        tempo = c;
    }

    for(int i=res.size()-1;i>=0;i--){
        const auto[a,b,c] = res[i];
        cout << idCidade[ordem[a]] << " do dia " << b << " ao dia " << c << endl;
    }


}

int main(){ 

    string nomeArquivoDados,nomeArquivoAcomodacoes,nomeArquivoVoos;

    nomeArquivoDados = "dadosEntrada.txt";
    nomeArquivoAcomodacoes = "acomodacoes.txt"; 
    nomeArquivoVoos = "voos.txt";

    map<string,int> cidadeId; // map para converter o nome da cidade em um representação numerica
    map<int,string> idCidade; // map para converter a representação numerica no nome da cidade
    int numCidade = 0;
    string origem;
    int quantDias;
    int quantCidades;
    vector<pair<int,int> > limiteDias; // quantidade minima de dias, quantidade maxima de dias
    string cidade,id;
    int chegada,saida;
    double valor;

    ifstream arquivoDados(nomeArquivoDados);

    if(arquivoDados.is_open()){

        arquivoDados>>origem;
        idCidade[numCidade] = origem;
        cidadeId[origem] = numCidade++;

        arquivoDados>>quantDias;
        quantDias++;
        
        arquivoDados>>quantCidades;
        quantCidades++;

        limiteDias.resize(quantCidades);

        for(int i=1;i<quantCidades;i++){
            string cidade;
            int minimoDias, maximoDias;
            arquivoDados>>cidade>>minimoDias>>maximoDias;
            limiteDias[numCidade] = {minimoDias,maximoDias};
            idCidade[numCidade] = cidade;
            cidadeId[cidade] = numCidade++;
        }

        arquivoDados.close();
    }else{
        cerr << "Erro ao abrir o arquivo: " << nomeArquivoDados << endl;
    }


    ifstream arquivoAcomodacoes(nomeArquivoAcomodacoes);
    //[id cidade, inicio, fim] = valor da acomodação      
    vector<vector<vector<double>>> acomodacoes(numCidade,vector<vector<double>>(quantDias,vector<double>(quantDias,INF)));

    if(arquivoAcomodacoes.is_open()){

        int quantAcomodacoes;
        arquivoAcomodacoes>>quantAcomodacoes;

        for(int i=0;i<quantAcomodacoes;i++){
            arquivoAcomodacoes>>id>>chegada>>saida>>cidade>>valor;
            if(cidadeId.find(cidade)==cidadeId.end()) continue;
            acomodacoes[cidadeId[cidade]][chegada][saida] = min(acomodacoes[cidadeId[cidade]][chegada][saida],valor);
        }

        arquivoAcomodacoes.close();

    }else{
        cerr << "Erro ao abrir o arquivo: " << nomeArquivoAcomodacoes << endl;
    }

    ifstream arquivoVoos(nomeArquivoVoos);
    //[id origem,id destino, dia] = valor
    vector<vector<vector<pdd>>> voos(numCidade,vector<vector<pdd>>(numCidade,vector<pdd>(quantDias,{INF,INF})));

    if(arquivoVoos.is_open()){

        int quantVoos;
        arquivoVoos>>quantVoos;        

        string cidadeOrigem, cidadeDestino;
        double tempoViagem;

        for(int i=0;i<quantVoos;i++){
            arquivoVoos>>id>>saida>>chegada>>cidadeOrigem>>cidadeDestino>>valor>>tempoViagem;
            if(cidadeId.find(cidadeOrigem) == cidadeId.end() || cidadeId.find(cidadeDestino)==cidadeId.end()) continue;
            if(saida!=chegada) continue; // simplificação inicial do problema
            voos[cidadeId[cidadeOrigem]][cidadeId[cidadeDestino]][saida] = min(voos[cidadeId[cidadeOrigem]][cidadeId[cidadeDestino]][saida],{valor,tempoViagem});
        }

        arquivoVoos.close();

    }else{
        cerr << "Erro ao abrir o arquivo: " << nomeArquivoVoos << endl;
    }



    vector<int> ordem(quantCidades-1);
    vector<int> melhorOrdem = ordem;

    for(int i=0;i<quantCidades-1;i++){
        ordem[i] = i+1;
    }

    double minCusto = INF;
    double tempo = INF;
    double limiteTempo = 9000;
    double epsilon = 1;


    while(true){

        minCusto = INF;
        tempo = INF;

        do{

            const auto [custo,tempoGasto] = solve2(ordem,acomodacoes,voos,limiteDias,quantDias,quantCidades,limiteTempo);

            if(minCusto > custo){
                minCusto = custo;
                melhorOrdem = ordem;
                tempo = tempoGasto;
            }

        
        }while(next_permutation(ordem.begin(),ordem.end()));

        if(tempo>=INF) break;

        cout << minCusto << " " << tempo << endl;

        for(int i=0;i<melhorOrdem.size();i++){
            cout << melhorOrdem[i] << " ";
        }
        cout << endl;

        imprimeRota2(melhorOrdem,acomodacoes,voos,limiteDias,quantDias,quantCidades,limiteTempo,idCidade);
        cout << "---------------\n";
        limiteTempo = tempo - epsilon;

    }



    return 0;
}