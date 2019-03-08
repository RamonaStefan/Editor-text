#include <iostream>       
#include <string.h>
#include<fstream>
#include <cstdio>
#include <stdlib.h>

using namespace std;


struct lista{ //structura nodurilor ce vor reprezenta fiecare caracter din text
    char caracter;
    struct lista *urmator;
    struct lista *precedent;
};

typedef struct lista LISTA;

struct node{  //structura nodurilor ce vor fi folosite in stiva pentru a contoriza istoria operatiilor
    char operatie[30];
    char sir1[30];
    char sir2[30];
    int val;
    struct node *urmator;
};

struct stiva{ //structura stivei
    struct node *cap;
    int lungime;
};

struct stiva* creare_stiva(){   //creare stiva
    struct stiva *stack = NULL;

    stack = new stiva;
    (*stack).lungime = 0;
    (*stack).cap = NULL;
}

struct node* creare_nod (char operatie[], int poz, char sir1[], char sir2[]){ //crearea nodurilor din stiva si alocarea informatiilor necesare
    struct node *nod= NULL;

    nod = new node;
    strcpy((*nod).operatie, operatie);
    (*nod).val = poz;
    strcpy((*nod).sir1, sir1);
    strcpy((*nod).sir2, sir2);
    (*nod).urmator = NULL;
    return nod;
}

void push(struct stiva *stack,char operatie[], int poz, char sir1[], char sir2[]){ //scoaterea cate unui elemnet din stiva 
    struct node *nod = NULL;

    nod = creare_nod(operatie,poz,sir1,sir2);
    (*nod).urmator = (*stack).cap;
    (*stack).cap = nod;
    (*stack).lungime++;
}

LISTA* move(LISTA *curent, LISTA *start, int *n, int lungime){ //mutarea cursorului
    int i;
    int pozitie = 0;
    LISTA *aux;

    aux = start;
    while (curent != aux) {
        pozitie++;			//se verifica pe ce pozitie se afla cursorul
        aux = (*aux).urmator;
    }
    if (*n >= 0){ 
        if (*n + pozitie <= lungime - 1) {			//se verifica daca deplasarea depaseste lungimea listei
            for (i = 0; i < *n; i++) {
                curent = (*curent).urmator;
            }
        }
        else {
            curent = (*start).precedent;
            *n = lungime - pozitie;				//se updateaza numarul miscarilor
        }
    }
    else {
        if (*n + pozitie >= 0) {				//se verifica daca deplasarea depaseste lungimea listei 
            for (i = 0; i< (-1)*(*n); i++) {
                curent = (*curent).precedent;
            }
        }
        else {
            curent = start;
            *n = pozitie*(-1);                                   //se updateaza numarul miscarilor
        }
    }
    return curent;
}

LISTA* undo_move (LISTA *start, LISTA *curent, int n, int lungime){		//inversul functiei move
    LISTA *aux;
    int pozitie = 0;
    int i;

    aux = start;
    while (curent != aux) {
	pozitie++;
	aux = (*aux).urmator;
    }
    if (n >= 0) {			// se verifica natura lui n pentru a determina sensul deplasarii(pozitiv-avanseaa, negativ-devanseaza)
        if (pozitie - n >= 0) {
            for (i = 0; i < n; i++) {
                curent = (*curent).precedent;
            }
        }
    }
    else {
        if (pozitie - n <= lungime-1) {
            for (i = 0; i < -n; i++) {
                curent = (*curent).urmator;
            }
        }
    }
    return curent;
}

LISTA* insert(LISTA *start, LISTA *curent, char sir[], int *lungime){				//functia aloca memorie pentru noine noduri si le leaga de lista
    LISTA *p = NULL;
    LISTA *q = NULL;
    int i;
    int lungime_sir;

    lungime_sir = strlen(sir);
    p = start;
    while (p != curent) {
        p = (*p).urmator;
    }
    p = (*p).precedent;
    for (i = 0; i < lungime_sir; i++){
        q = new LISTA;
        if (q == NULL) {
            cout << "Alocare nereusita.\n";
            exit(1);
        }
        (*q).caracter = sir[i];
        (*q).urmator = (*p).urmator;
        (*q).precedent = p;
        (*(*p).urmator).precedent = q;
        (*p).urmator = q;
        p = q;
        *lungime = *lungime + 1;
    }
    p = (*p).urmator;
    return p;
}

LISTA* del(LISTA *curent, int n, int *lungime, char sir[]){		//sterge un numar n de noduri din lista si salveaza ce a sters in char sir[]
    LISTA *aux = NULL;
    int i;

    for (i = 0; i < n; i++) {
        aux = curent;
        sir[i] = (*curent).caracter;
        (*(*curent).urmator).precedent = (*curent).precedent;
        (*(*curent).precedent).urmator = (*curent).urmator;
        curent = (*curent).urmator;
        delete aux;
        *lungime = *lungime - 1;
    }
    sir[n] = '\0';
    return curent;
}

LISTA* undo_del(LISTA *start, LISTA *curent, int n, int *lungime, char sir[]) {   //inverseaza functia delprin  apelarea functiei insert 
    int i;

    curent = insert(start,curent,sir, lungime);
    for (i = 0; i < strlen(sir); i++) {
        curent = (*curent).precedent;
    }
    return curent;
}

LISTA* undo_insert(LISTA *start, LISTA *curent, char sir[], int *lungime){		//inverseaza functia insert prin apelarea functiei del
    int i;
    int lungime_sir;

    lungime_sir = strlen(sir); 
    for (i = 0; i < lungime_sir; i++) {
        curent = (*curent).precedent;
    }
    curent = del(curent, lungime_sir, lungime, sir);
    return curent;
}

LISTA *copy(LISTA *curent, int n, LISTA *start2){  			//copiaza o secventa de noduri intr-o noua lista, si se salveaza pointerul catre primul element
    LISTA *aux = NULL;
    LISTA *q = NULL;
    int i;

    (*start2).caracter = (*curent).caracter;
    (*start2).urmator = NULL;
    (*start2).precedent = NULL;
    q = start2;
    curent = (*curent).urmator;
    for (i = 1; i < n; i++) {
        aux = new LISTA;						//se aloca memorie pentru fiecare nod
        if (aux == NULL) {
            cout << "Alocare dinamica nereusita" << endl;
            exit(1);
        }
        (*aux).caracter = (*curent).caracter;
        (*aux).urmator = NULL;
        (*aux).precedent = q;
        (*q).urmator = aux;
        q = aux;
        curent = (*curent).urmator;
    }
    (*q).urmator = start2;
    (*start2).precedent = q;
    return start2;
}

LISTA*  paste(LISTA **start,LISTA *start2, LISTA *curent, int *lungime, LISTA **inceput){		//insereaza lista copiata in cea existenta
    LISTA *q, *aux, *p;

    q = start2;
    p = new LISTA;					//se creaza inca o lista pentru o ulterioara apelarea a functiei paste
    if (p == NULL) {
        cout << "Alocare dinamica nereusita" << endl;
        exit(1);
    }
    (*p).caracter = (*q).caracter;
    (*p).urmator = NULL;
    (*p).precedent = NULL;
    *inceput = p;
    q = (*q).urmator;
    while(q != start2) {
        aux = new LISTA;
        if (aux == NULL) {
            cout << "Alocare dinamica nereusita" << endl;
            exit(1);
        }
        (*aux).caracter = (*q).caracter;
        (*aux).urmator = NULL;
        (*aux).precedent = p;
        (*p).urmator = aux;
        p = aux;
        q = (*q).urmator;
    }
    (*aux).urmator = *inceput;
    (**inceput).precedent = aux;
   if (curent != (**start).precedent) {					//se leaga legaturile
        q = curent;
        curent = (*curent).precedent;
        p = *inceput;
        (*curent).urmator = p;
        (*p).precedent = curent;
        *lungime = *lungime + 1;
        p = (*p).urmator;
        while ((*p).urmator != *inceput) {
            p = (*p).urmator;
            *lungime = *lungime + 1;
        }
        (*p).urmator = q;
        (*q).precedent = p;
        curent = q;
    }
    else {
        q = (*curent).urmator;
        p = *inceput;
        (*curent).urmator = p;
        (*p).precedent = curent;
        *lungime = *lungime + 1;
        p = (*p).urmator;
        while ((*p).urmator != *inceput) {
            p = (*p).urmator;
            *lungime = *lungime + 1;
        }
        (*p).urmator = q;
        (*q).precedent = p;
        curent = (*q).precedent;
    }
    return curent;
}

LISTA* undo_paste (LISTA *start, LISTA *curent, int *lungime,LISTA *inceput){		//se elimina nodurile introduse prin ruperea legaturilor
    LISTA  *aux, *q;

    if ((*start).precedent != curent) {
	aux = curent;
	while (curent != inceput) {
	    curent = (*curent).precedent;
	    *lungime = *lungime-1;
	}
	curent = (*curent).precedent;
	*lungime = *lungime-1;
	(*curent).urmator = aux;
	(*inceput).precedent = (*aux).precedent;
	(*(*aux).precedent).urmator = inceput;
	(*aux).precedent = curent;
	curent = aux;
    }
    else {
	aux = (*curent).urmator;
	while (curent != inceput) {
	    curent = (*curent).precedent;
	    *lungime = *lungime-1;
	}
	*lungime = *lungime-1;
	curent = (*curent).precedent;
	(*curent).urmator = aux;
	(*inceput).precedent = (*aux).precedent;
        (*(*aux).precedent).urmator = inceput;
        (*aux).precedent = curent;
	curent = (*aux).precedent;
    }
    aux = inceput;				//se elibereaza memoria
    while((*inceput).precedent != aux) {
	q = aux;
	aux = (*aux).urmator;
	delete q;
    }
    q = aux; 
    aux = (*aux).urmator; 
    delete q; 

    return curent;
}

LISTA* backspace(LISTA *start, LISTA *curent, int *lungime, char *c){		//stergerea caracterului anterior celui curent si eliberarea memoriei
    LISTA *aux;

    if(curent == start) {
        return curent;
    }
    if (curent != (*start).precedent) {
        curent = (*curent).precedent;
    }
    
    aux = curent;
    *c = (*curent).caracter;			//caracterul este trimis cu referinta pentru a fi retinut
    (*(*curent).urmator).precedent = (*curent).precedent;
    (*(*curent).precedent).urmator = (*curent).urmator;
    curent = (*aux).urmator;
    delete aux;
    *lungime = *lungime - 1;
    return curent;
}

LISTA* undo_backspace (LISTA *curent, int *lungime, char c){		//inserarea caracterului sters
    LISTA *q;

    curent = (*curent).precedent;
    q = new LISTA;
    if (q == NULL) {
        cout << "Alocare nereusita.\n";
        exit(1);
    }
    (*q).caracter = c;
    (*q).urmator = (*curent).urmator;
    (*q).precedent = curent;
    (*(*curent).urmator).precedent = q;
    (*curent).urmator = q;
    *lungime = *lungime + 1;
    curent = (*curent).urmator;
    curent = (*curent).urmator;
    return curent;
}

LISTA* replace(LISTA *start, char sir_vechi[], char sir_nou[], int *lungime){ 		// updatarea continutului unei secvente de noduri si eventual
    LISTA *p, *poz;									// adaugarea si/sau eliminarea de noduri
    int j;
    int comun = 0, ok = 0;
    int lvechi, lnou;
    char c[30];

    lnou = strlen(sir_nou);
    lvechi = strlen(sir_vechi);
    p = start;
    poz = p;
    do {
        ok = 0;
        while ((*p).caracter != sir_vechi[0] && p != (*start).precedent) {
            p = (*p).urmator;
            poz = p;
        }
        if ((*p).caracter == sir_vechi[0]) {
            ok = 1;
        }
        if (ok == 1){
            comun = 0;
            for (j = 0; j < lvechi; j++) {
                if ((*p).caracter == sir_vechi[j]) {
                    comun++;
                    p = (*p).urmator;
                }
            }
            if (comun == lvechi) {					//se verifica diferenta intre lungimile celor doua siruri
                if (lvechi == lnou) {
                    for (j = 0; j <lnou; j++) {
                        (*poz).caracter = sir_nou[j];
                        poz = (*poz).urmator;
                    }
                }
                else if (lvechi > lnou) {
                    for (j = 0; j < lnou; j++) {
                        (*poz).caracter = sir_nou[j];
                        poz = (*poz).urmator;
                    }
                    poz = del(poz,lvechi-lnou, lungime,c);
                }
                else {
                    for (j = 0; j < strlen(sir_vechi); j++) {
                        (*poz).caracter = sir_nou[j];
                        poz = (*poz).urmator;
                    }
                    poz = insert(start, poz, sir_nou + strlen(sir_vechi), lungime);
                }
            }
        }
    } while (ok == 1);
    return start;
}

int main(int argc, char *argv[])
{
    LISTA *p = NULL, *q = NULL, *cap_lista = NULL, *cap_lista2 = NULL, *inceput=NULL;
    char *s, c, linie[85], operatie[20] = "", sir[30], *cuv[20], sir1[30] = "", sir2[30] = "";
    int lungime = 0, i = 0, nr = 0, k = 0, oper = 0, L= 0;
    struct stiva *stack = creare_stiva();
    struct stiva *stack2 = creare_stiva();
    struct node *ajutor;
   	
    inceput = new LISTA;
    if (inceput == NULL) {
        cout << "Alocare dinamica nereusita." << endl;
        exit(1);
    }
    ifstream f(argv[1]);
    ifstream g(argv[2]);
    ofstream r(argv[3]);
    if (!f) {
        cout << "Deschidere nereusita date.in" << endl;
        exit(1);
    }
    if (!g) {
        cout << "Deschidere nereusita operatii.in" << endl;
        exit(1);
    }
    if (!r) {
        cout << "Deschidere nereusita rezultat.out" << endl;
        exit(1);
    }
    //citirea datelor din fisierul date.in
    f.get(c);
    p = new LISTA;
    if (p == NULL) {
        cout << "Alocare nereusita" << endl;
        exit(1);
    }	
    (*p).caracter = c;
    (*p).urmator = NULL;
    (*p).precedent = NULL;
    cap_lista = p;
    lungime++;
    while (f.get(c)) {
        q = new LISTA;
        if (q == NULL) {
            cout << "Alocare nereusita" << endl;
            exit(1);
        }
        (*q).caracter = c;
        (*q).urmator = NULL;
        (*q).precedent = p;
        (*p).urmator = q;
        p = q;
        lungime++;
    }
    (*q).urmator = cap_lista;
    (*cap_lista).precedent = q;
    p = cap_lista;
    f.close();
    //citirea operatilor din fisierul operatii.in si efectuarea lor prin apelarea functiilor aferente 
    nr = 0;
    g.getline(linie, 80);
    L = strlen(linie);
    for (i = 0; i < L; i++) {
       	nr = nr*10+linie[i]-'0';		//determinarea numarului de operatii
    }
    for (i = 0; i < nr; i++) {
       	g.getline(linie, 80);
       	s = strtok(linie, ".,!?:; ");    //citirea linie cu linie si imparitrea in cuvinte cu ajutorul strtok
       	k = 0;
       	while (s != NULL) {
            cuv[k] = s;
            k++;
            s = strtok(NULL, ".,!?:; ");
        }
        if (strcmp(cuv[0], "move") == 0 || strcmp (cuv[0], "MOVE") == 0) {
            oper = 0;
            L = strlen(cuv[1]);
            if (cuv[1][0] == '-') {
                for (k = 1; k < L; k++) {
                    oper = oper*10 + cuv[1][k]-'0';
                }
                oper = -oper;
            }
	   else {
           for(k = 0; k < L; k++) {
               oper = oper*10+cuv[1][k]-'0';
           }
	   }
           push(stack,cuv[0], oper, sir1, sir2);
           p = move(p, cap_lista, &oper, lungime);
        }
	if (strcmp(cuv[0], "insert") == 0 || strcmp (cuv[0], "INSERT") == 0) {
            push(stack,cuv[0],0,cuv[1],sir2);
            if (p == cap_lista) {
               	p = insert(cap_lista,p, cuv[1], &lungime);
                q = p;
                L = strlen(cuv[1]);
                for (k = 0; k < L; k++) {
                    q = (*q).precedent;
                }
                cap_lista = q;
            }
            else {
                p = insert(cap_lista,p, cuv[1], &lungime);
            }
	}
        if (strcmp(cuv[0], "copy") == 0 || strcmp (cuv[0], "COPY") == 0) {
            oper = 0;
            L = strlen(cuv[1]);
            for (k = 0; k < L; k++) {
                oper = oper*10 + cuv[1][k]-'0';
	    }            	
            cap_lista2 = new LISTA;
            if (cap_lista2 == NULL) {
                cout << "Alocare dinamica nereusita." << endl;
                exit(1);
            }
            copy(p, oper, cap_lista2);
        }
        if (strcmp(cuv[0], "paste") == 0 || strcmp (cuv[0], "PASTE") == 0) {
	    push(stack,cuv[0], 0, sir1, sir2);
            if (p == cap_lista) {
                p = paste(&cap_lista, cap_lista2, p, &lungime, &inceput);
                cap_lista = inceput;
            }
            else  {
                p = paste(&cap_lista, cap_lista2,p,&lungime, &inceput);
            }
        }
        if (strcmp(cuv[0], "backspace") == 0 || strcmp (cuv[0], "BACKSPACE") == 0) {
	    if (p != cap_lista) {
	        p = backspace(cap_lista, p, &lungime, &c);
		sir1[0] = c;
		sir1[1] = '\0';
		if (p == cap_lista) {
		    p = (*p).precedent;
	  	}	
	        push(stack,cuv[0], 1, sir1, sir2);
	        sir1[0] = '\0';
	    } 	
        }
        if (strcmp(cuv[0], "del") == 0 || strcmp (cuv[0], "DEL") == 0){
            oper = 0;
            L = strlen(cuv[1]);
            for (k = 0; k < L; k++) {
                oper = oper*10 + cuv[1][k]-'0';
	    }            	
            if (p == cap_lista) {
                p = del(p, oper, &lungime, sir);
      		cap_lista = p;
            }
            else {
                p = del(p, oper, &lungime, sir);
	    }
	    push(stack,cuv[0], oper,sir, sir2);
        }
        if (strcmp(cuv[0], "replace") == 0 || strcmp (cuv[0], "REPLACE") == 0) {
	    push(stack,cuv[0], oper,cuv[1],cuv[2]);
            cap_lista = replace(cap_lista, cuv[1], cuv[2], &lungime);
        }
        if (strcmp(cuv[0], "undo") == 0 || strcmp (cuv[0], "UNDO") == 0) {
	    if ((*stack).cap != NULL) {
                strcpy(operatie, (*(*stack).cap).operatie);
                strcpy(sir1, (*(*stack).cap).sir1);
                strcpy(sir2, (*(*stack).cap).sir2);
                oper = (*(*stack).cap).val;
	        L = strlen(sir1);
                if (strcmp(operatie, "move") == 0 || strcmp (operatie, "MOVE") == 0) {
                    p = undo_move(cap_lista, p, oper, lungime);
                }
                if (strcmp(operatie, "insert") == 0 || strcmp (operatie, "INSERT") == 0) {
		    q = p;                
		    for (k = 0; k < L ;k++) {
        	        q = (*q).precedent;
                    }
		    if (q == cap_lista) {
		        p = undo_insert(cap_lista,p,sir1,&lungime);
		        cap_lista = p;
		    }
		    else {
		        p = undo_insert(cap_lista,p,sir1,&lungime);
		    }
	        }
                if (strcmp(operatie, "paste") == 0 || strcmp (operatie, "PASTE") == 0) {
		    if (inceput == cap_lista) {
		        p = undo_paste(cap_lista2,p,&lungime, inceput);
		        cap_lista = p;
		    }
		    else {
		        p = undo_paste(cap_lista2,p,&lungime, inceput);
		    }
	        }
                if (strcmp(operatie, "backspace") == 0 || strcmp (operatie, "BACKSPACE") == 0) {
                    p = undo_backspace(p, &lungime, sir1[0]);
                }
                if (strcmp(operatie, "del") == 0 || strcmp (operatie, "DEL") == 0) {
                    if (p != cap_lista) {
		        p = undo_del(cap_lista, p,oper,&lungime,sir1);
		    }
		    else {
		        p = undo_del(cap_lista, p, oper, &lungime, sir1);
		        cap_lista = p;
		    }
                }
                if (strcmp(operatie, "replace") == 0 || strcmp (operatie, "REPLACE") == 0) {
                    cap_lista = replace(cap_lista, sir2, sir1, &lungime);
                }
                push(stack2,operatie, oper,sir1,sir2);
                (*stack).cap = (*(*stack).cap).urmator;
                (*stack).lungime--;
            }
        }
        if (strcmp(cuv[0], "redo") == 0 || strcmp (cuv[0], "REDO") == 0) {
	    if((*stack2).cap != NULL) {
         	strcpy(operatie, (*(*stack2).cap).operatie);
            oper = (*(*stack2).cap).val;
            strcpy(sir1, (*(*stack2).cap).sir1);
            strcpy(sir2, (*(*stack2).cap).sir2);
            if (strcmp(operatie, "move") == 0 || strcmp (operatie, "MOVE") == 0) {
                p = move(p, cap_lista, &oper, lungime);
            }
            if (strcmp(operatie, "insert") == 0 || strcmp (operatie, "INSERT") == 0) {
                if (p == cap_lista) {
                    p = insert(cap_lista,p, sir1, &lungime);
                    q = p;
                    L = strlen(cuv[1]);
                    for (k = 0; k < L; k++) {
                         q = (*q).precedent;
                    }
                    cap_lista = q;
                }
                else {
                    p = insert(cap_lista,p, sir1, &lungime);
                }
            }
            if (strcmp(operatie, "paste") == 0 || strcmp (operatie, "PASTE") == 0) {
        	if (p == cap_lista) {
            	    p = paste(&cap_lista, cap_lista2, p, &lungime, &inceput);
            	    cap_lista = inceput;
        	}
                else  {
                    p = paste(&cap_lista, cap_lista2,p,&lungime, &inceput);
                }
            }
            if (strcmp(operatie, "backspace") == 0 || strcmp (operatie, "BACKSPACE") == 0) {
                if(p != cap_lista) {
                    p = backspace(cap_lista, p, &lungime, &sir1[0]);
                    if (p == cap_lista) {
                        p = (*p).precedent;
                    }
                }
            }
            if (strcmp(operatie, "del") == 0 || strcmp (operatie, "DEL") == 0) {
            	if (p == cap_lista) {
                    p = del(p, oper, &lungime, sir1);
                    cap_lista = p;
                }
                else {
                    p = del(p, oper, &lungime, sir1);
                }
            }
            if (strcmp(operatie, "replace") == 0 || strcmp (operatie, "REPLACE") == 0) {
                cap_lista = replace(cap_lista, sir1, sir2, &lungime);
            }
            push(stack,operatie, oper,sir1, sir2);
            (*stack2).cap = (*(*stack2).cap).urmator;
            (*stack2).lungime--;
	    }
        }
    }
//eliberarea nemoriei folosite

    while ((*stack).cap != NULL) {
        ajutor = (*stack).cap;
        (*stack).cap = (*(*stack).cap). urmator;
        delete ajutor;
    }
    delete stack;
    while ((*stack2).cap != NULL) {
        ajutor = (*stack2).cap;
        (*stack2).cap = (*(*stack2).cap). urmator;
        delete ajutor;
    }
    delete stack2;
    if(cap_lista2 != NULL) {
        p = cap_lista2;
        p= (*p).urmator;
        while (cap_lista2 != p) {
            q = p;
            p = (*p).urmator;
            delete q;
        }
        delete p;
    }
   //scrierea in fisierul rezultat.out si eliberarea memoriei
    p = cap_lista;
    q = p;
    r<< (*cap_lista).caracter;
    p = (*p).urmator;
    delete q;
    while (p != cap_lista) {
        q = p;
        r<< (*p).caracter;
        p = (*p).urmator;
        delete q;
    }

    r.close();
    g.close();
    return 0;
}


