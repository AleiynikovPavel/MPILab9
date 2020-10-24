#include <mpi.h>

#define APPEND_SIZE 60

//Формируем сообщение очередного бухгалтера в отчёте
void appendMessage(char * data, int rank);
//Инициализируем область массива нулями
void initArray(char * data, int offset, int size);
//Получаем число доходов
float getIncome(char * data);
//Получаем число расходов
float getCoast(char * data);
//Генерируем случайное число с двумя знаками после запятой
float getRand();

int main( int argc, char **argv ) {
    int rank, size;
    MPI_Status status;
    int errCode = MPI_Init(&argc, &argv);
    if (errCode != 0) {
        printf("Error init %d", errCode);
        return -1;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size < 2) {
        printf("Requires at least 2 processes\n");
        return -1;
    }
    char *WR = new char[APPEND_SIZE * (rank + 1)];
    if (rank == 0) {
        initArray(WR, 0, APPEND_SIZE);
        appendMessage(WR, rank);
        MPI_Send(WR, APPEND_SIZE, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
    } else {
        initArray(WR, APPEND_SIZE * rank, APPEND_SIZE * (rank + 1));
        MPI_Recv(WR, APPEND_SIZE * rank, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, &status);
        printf("Buh %d receive: \n %s", rank, WR + APPEND_SIZE * (rank - 1));
        if (rank + 1 != size) {
            appendMessage(WR, rank);
            MPI_Send(WR, APPEND_SIZE * (rank + 1), MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
        } else {
            double sumI = 0, sumC = 0;
            printf("\nResult: \n");
            for (int i = 0; i < size - 1; i++) {
                sumI += getIncome(WR + APPEND_SIZE * i);
                sumC += getCoast(WR + APPEND_SIZE * i);
                printf("%s", WR + APPEND_SIZE * i);
            }
            printf("\nBuh %d SUM: I: %.2f C: %.2f\n", rank, sumI, sumC);
        }
    }
    delete[] WR;
    MPI_Finalize();
    return 0;
}

void appendMessage(char * data, int rank) {
    srand(time(NULL) / (rank + 1));
    sprintf(data + APPEND_SIZE * rank, "Buh: %d I: %.2f C: %.2f\n", rank, getRand(), getRand());
}

void initArray(char * data, int offset, int size) {
    for (int i = offset; i < size; i++) {
        data[i] = 0;
    }
}

float getIncome(char * data) {
    char *strI = strchr(data, 'I');
    strI += 2;
    return atof(strI);
}

float getCoast(char * data) {
    char *strC = strchr(data, 'C');
    strC += 2;
    return atof(strC);
}

float getRand() {
    return rand() / 100.f;
}

