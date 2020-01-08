int a,b,c;
int a,b,c;// 1 v redefine

int afun(float a, float a)// 2 para redefine
{

}

int afun(float a, int x)//3 afun redefine
{
	float b;
	return b;//4 return value error
}

int main() 
{
	int m=1;
	float b;
	int x;
	y = 1;//5 y undefined
	afun(m);//6 afun para type unmatch
	afun(b);//7 afun para few
	afun(b,m,x);//8 afun para many
	x();//9 x is not a func
	5++;//10 ++ must be left-value
	5 += m;//11 += must be left-value
	b = m;//12 type unmatch
	5 = b;//13 assign must be left-value
	ab(); //14 func undefined
	break;//15 cannot break here
	while(m){
		//break;
	}
	return 1;
}
