#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<algorithm>
#include<fstream>
#include <cstring>  
#include "readCsv.cpp"

#define QTD_ATTRIBUTES 16

struct type_info{
    int type;
    int qtd;
};

class point{
    private:
        double *attributes;
        int type;
    public: 
        point(){
            this->attributes = new double [QTD_ATTRIBUTES];
        }
        point(std::vector<double> vet){

            for(int i = 0; i < QTD_ATTRIBUTES; i++){
                this->attributes[i] = vet[i];
            }
            this->type = vet[QTD_ATTRIBUTES];
        }

        void set_attributes(double attributes[QTD_ATTRIBUTES]){
            this->attributes = attributes;
        }

        void set_type(int type){
            this->type = type;
        }

        int get_type(){
            return this->type;
        }

        double get_attribute(int pos){
            return attributes[pos];
        }

        double calculate_distance(point other_point){
            double total = 0;
            for(int i = 0; i < QTD_ATTRIBUTES; i++){
                double diference = attributes[i] - other_point.get_attribute(i);
                total += diference * diference;
            }
            return sqrt(total);
        }
};

class list{
    private:
        std::vector<point> my_list;
        int num_neighbours;
        point pivo;

    public:
        list(int num_neighbours){
            this->num_neighbours = num_neighbours;
        }

        void add_pivo(point pivo){
            this->pivo = pivo;
        }

        void add(point new_neighbourd){
            double distance = this->pivo.calculate_distance(new_neighbourd);
            int msize = this->my_list.size();
            
            if(this->my_list.size() == 0 || distance > this->my_list[this->my_list.size()-1].calculate_distance(pivo)){
                if(this->my_list.size() == this->num_neighbours){
                    this->my_list.pop_back();
                }
                this->incert_ordered_in_list(new_neighbourd);
            }

        }

        void incert_ordered_in_list(point new_neighbourd){
            int i = this->my_list.size()-1;
            double new_distance = this->pivo.calculate_distance(new_neighbourd);
            while(i >= 0){
                if(new_distance > this->pivo.calculate_distance(this->my_list[i])){
                   this->my_list.insert(this->my_list.begin() + i+1 ,new_neighbourd);
                   break;
                }
                i--;
            }
            if(i == -1){
                this->my_list.insert(this->my_list.begin(), new_neighbourd);
                
            }
        }

        void define_type(){
            std::vector<type_info> species_present;

            for (int i = 0; i < this->my_list.size(); i++){
                int pos = this->pos(this->my_list[i].get_type(), species_present);
                if( pos == -1){
                    type_info new_type_info;
                    new_type_info.type = this->my_list[i].get_type();
                    new_type_info.qtd = 1;
                    species_present.push_back(new_type_info);
                }
                else{
                    species_present[pos].qtd++;
                }
            }

            int closest = 0;
            for (int i = 1; i < species_present.size(); i++){
                if(species_present[closest].qtd < species_present[i].qtd){
                    closest = i;
                }
            }

        }

        int pos(int target, std::vector<type_info> species_present){
            for (int i = 0; i < species_present.size(); i++){
                if(species_present[i].type == target){
                    return i;
                }
            }
            return -1;
        }

        int count(int target){
            int qtd = 0;
            for (int i = 0; i < this->my_list.size(); i++){
                if(this->my_list[i].get_type() == target){
                    qtd++;
                }
            }
            return qtd;
        }
};

int main(){
    std::vector<point> list_of_point;
    std::vector<std::vector<double>> lines;
    readCsv(lines);
    random_shuffle(lines.begin(), lines.end());
    int porcentage = 70;
    int qtd_train = (lines.size()/10)*7;
    int pos_type = QTD_ATTRIBUTES;
    int num_neighbours = 5;

    for(int i = 0; i < qtd_train; i++){
        
        double* att = new double [QTD_ATTRIBUTES];
        for(int j = 0; j < QTD_ATTRIBUTES - 1 ; j++){
            att[j] = lines[0][j];
        }
        point new_point;
        new_point.set_attributes(att);
        int new_point_type = (int)lines[0][pos_type];
        new_point.set_type(new_point_type);
        list_of_point.push_back(new_point);
        lines.erase(lines.begin());
    }




    while(lines.size() > 0){

        // std::cout << "\n\n ------------------------------------------------ \n\n ";
        // for(int i = 0; i < lines.size(); i++){
        //     std::cout << i << "-"<<lines[i][16] << "\t";
        // }

        std::vector<double> thos_line = lines[0];
        list lista(num_neighbours);
        point new_point(thos_line);
        lines.erase(lines.begin());
        lista.add_pivo(new_point);

        for(int i = 0; i < list_of_point.size(); i++){
            lista.add(list_of_point[i]);
        }
        lista.define_type();

        
    }
    /*  def test_algorithm(self):
        test_nodes = self.make_nodes(self.test_population)
        i = 0
        for node in test_nodes:
            minha_lista = Lista()
            minha_lista.add_pivo(node)
            for node in self.nodes:
                minha_lista.adiciona(node)
            minha_lista.define_specie()
            
            if minha_lista:
                self.nodes.append(minha_lista.pivo)
            if minha_lista.pivo.specie != self.test_population.get(["Species"]).iloc[i,0]:
                print("DIFERENTE")
            i += 1
        
    
    */
}
