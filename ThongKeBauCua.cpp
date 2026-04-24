#include<iostream>
#include<vector>
#include <random>
using namespace std;
class NguoiChoi{
private:
    int strategy[64][6];/* stratedy[i][j]: có thể mang 2 trạng thái 0 hoặc 1. 
Nếu strate[i][j]=1 thì chiến thuật thứ i chọn con vật thứ j, ngược lại strate[i][j]=0 thì chiến thuật thứ i không chọn con vật thứ j*/
    int point[64]; // point[i] là số điểm mà chiến thuật thứ i có được
    int betCount[64]; //beCount[i] là số lượng con vật mà chiến thuật i cược
    int win[64];
public:
    //Phương thức khởi tạo các thuộc tính 
    NguoiChoi(){
        for(int i=0;i<(1<<6);i++){
            betCount[i]=0;
            point[i]=0;
            win[i] = 0;
            for(int j=0;j<6;j++){
                strategy[i][j]=0;
                if((i>>j)&1){
                    strategy[i][j]=1;
                    betCount[i]++;
                }
            }
        }
    }
    // Phương thức lấy mảng 2 chiều strategy từ người chơi
    int (* get_strategy())[6]{
        return strategy;
    }
    // Phương thức lấy mảng 1 chiều point từ người chơi
    int *get_point(){
        return point;
    }
    // Phương thức lấy mảng 1 chiều betCount từ người chơi
    int *get_betCount(){
        return betCount;
    }
    int *get_win(){
        return win;
    }

};
class NhaCai{
private:
    int dice[3]; // thuộc tính lưu trữ 3 con xúc xắc
    mt19937 gen;
public:
    NhaCai() {
        // Tạo nguồn random
        random_device rd;
        gen = mt19937(rd());
    }
    //Phương thức xốc 3 con xúc xắc với xác suất của các mặt là như nhau
    void roll_dice1(){ 
        // Phân phối đều từ 0 đến 5
        uniform_int_distribution<> dist(0, 5);
        // Random 3 con xúc xắc 
        dice[0] = dist(gen);
        dice[1] = dist(gen);
        dice[2] = dist(gen);
    }
    //Phương thức xốc 3 con xúc xắc với xác suất của ra mặt 0 (NAI) gấp đôi các mặt còn lại
    void roll_dice2(){
        // Trọng số: mặt 0 gấp đôi
        discrete_distribution<> dist({2, 1, 1, 1, 1, 1});
        dice[0] = dist(gen);
        dice[1] = dist(gen);
        dice[2] = dist(gen);
    }
    void check(NguoiChoi &nguoichoi){
        int (*stratedy)[6]=nguoichoi.get_strategy();
        int *point = nguoichoi.get_point();
        int *betCount = nguoichoi.get_betCount();
        int *win = nguoichoi.get_win();
        for(int i=0;i<64;i++){
            int gain = 0; // số mà chiến thuật thứ i điểm đạt được
            int loss=betCount[i]; // số lượng con vật mà chiến thuật thứ i cược
            if(stratedy[i][dice[0]]){ // Kiểm tra xem chiến thuật thứ i có chọn con vật trong xúc xắc đầu tiên hay không
                gain++;
                loss--;
            }
            if(stratedy[i][dice[1]]){// Kiểm tra xem chiến thuật thứ i có chọn con vật trong xúc xắc thứ hai hay không
                gain++;
                if(dice[1]!=dice[0]){
                    loss--;
                }
            }
            if(stratedy[i][dice[2]]){// Kiểm tra xem chiến thuật thứ i có chọn con vật trong xúc xắc thứ ba hay không
                gain++;
                if(dice[2]!=dice[0]&&dice[2]!=dice[1]){
                    loss--;
                }
            }
            if(gain-loss>0){ // gain-loss>0 tức là sau ván chơi này chiến thuật thứ i có lời => chiến thuật thứ i thắng ván này
                win[i]++;
            }
            point[i]+=gain-loss; //gain-loss chính là số điểm mà chiến thuật thứ i nhận được sau ván chơi
        }
    }
};
int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    freopen("test.out","w",stdout);
    NhaCai A;
    NguoiChoi B;
    //Chạy mô phỏng 1 triệu ván chơi
    int round=10000000;
    for(int i=1;i<=round;i++){
        A.roll_dice2(); // Nhà cái xốc xúc xắc
        A.check(B); // Xử lý các chiến thuật của người chơi
    }
    string tenLinhVat[] = {"Nai", "Bau", "Ga", "Ca", "Cua", "Tom"};
    int *point = B.get_point();
    int *win = B.get_win();
    //In ra lần lượt là chiến thuật chơi, số điểm có được, tỷ lệ thắng và giá trị kỳ vọng sau  khi kết thúc trò chơi
    for(int i=1;i<64;i++){
        for(int j=0;j<6;j++){
            if((i>>j)&1){
                cout<<tenLinhVat[j]<<' ';
            }
        }
        cout<<"| "<<point[i]<<" | "<<(double)win[i]/round<<" | "<<(double)point[i]/round<<'\n';
    }
    return 0;
}