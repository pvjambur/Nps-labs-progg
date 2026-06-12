# 3rd program - error detection and correction

```c
# CHECKSUM

#include<stdio.h>

unsigned fields[10];
 
unsigned short checksum(){
    int i;
    int sum=0;
    printf("Enter IP header information in 16 bit words\n");

    for(i=0; i<10;i++){
        // printf("Field %d\n", i+1);
        scanf("%x", &fields[i]);
        sum = sum + (unsigned short) fields[i];
        while (sum>>16)
            sum = (sum & 0xFFFF) + (sum >> 16);
    }

    sum =~ sum;

    return (unsigned short)sum;
}

int main(){
    unsigned short result1, result2;

    //Sender
    result1 = checksum();
    printf("\nComputer Checksum at sender: %x\n", result1);
    //Receiver
    result2 = checksum();
    printf("\nComputerd Checksum at receiver: %x\n", result2);

    if (result2 == 0x0000)
        printf("No error");
    else
        printf("Error in Data received");
    
    return 0;
}
```

```c
#HAMMING

#include<stdio.h>
int main()
{
	int data[8],c,c0,c1,c2,recv[8],i;
	printf("Enter four bit data ");
	scanf("%d",&data[3]);
	scanf("%d",&data[5]);
	scanf("%d",&data[6]);
	scanf("%d",&data[7]);
	data[1]=(data[3]+data[5]+data[7])%2;
	data[2]=(data[3]+data[6]+data[7])%2;
	data[4]=(data[5]+data[6]+data[7])%2;
	printf("The Data encoded is :");
	for(i=1;i<8;i++){
		printf("%d",data[i]);
	}
	printf("\nEnter Data received is : ");
	for(i=1;i<8;i++){
		scanf("%d",&recv[i]);}
	c0=	(recv[1]+recv[3]+recv[5]+recv[7])%2;
	c1=	(recv[2]+recv[3]+recv[6]+recv[7])%2;
	c2=	(recv[4]+recv[5]+recv[6]+recv[7])%2;
	c=c2*4+c1*2+c0*1;
	printf("The error is at position %d",c);
	if(recv[c]==0)
		recv[c]=1;
	else 
		recv[c]=0;
	printf("\nThe Data corrected is :");
	for(i=1;i<8;i++){
		printf("%d",recv[i]);
	}
	
	return 0;

}
```

```c
HAMMING 

✅ Sample Input (No Error Case)
🔹 Step 1: Enter 4-bit data
Enter four bit data
1 0 1 1

👉 The program will encode it (example output):
The Data encoded is :0110011

🔹 Step 2: Enter received data (same as encoded)
Enter Data received is :
0 1 1 0 0 1 1

👉 Output:
The error is at position 0
The Data corrected is :0110011

✔ No error detected

❌ Sample Input (With Error)
🔹 Step 1: Enter same data
1 0 1 1

👉 Encoded:
0110011

🔹 Step 2: Introduce error (flip one bit)
(Change 3rd bit from 1 → 0)
Enter Data received is :
0 1 0 0 0 1 1

👉 Output:
The error is at position 3
The Data corrected is :0110011

✔ Error detected and corrected
```