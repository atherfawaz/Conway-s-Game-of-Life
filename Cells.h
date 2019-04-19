typedef struct {
	int alive, died;
    long int maintained;
} Cells;

Cells cellInitialization() {
    Cells temp;
    temp.alive = temp.maintained = temp.died = 0;
    return temp;
}
