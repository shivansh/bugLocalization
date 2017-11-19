data<-read.csv("output.csv")
print(data)
data<-data[2:nrow(data),1:ncol(data)]
colnames(data)[colnames(data)=="TestCase"]<-"LineNumber"
print(data)
rownames(data)<-seq(length=nrow(data))
print(typeof(data[1,5]))
print("Started Loop")
df<-data.frame(LineNumber=data[,1],Hit0=integer(nrow(data)),Hit1=integer(nrow(data)),Hit2=integer(nrow(data)), Hit3=integer(nrow(data)))
for(i in 1:nrow(df)){
    for(j in 2:ncol(data)){
        df[rownames(df)[i],colnames(df)[data[rownames(data)[i],colnames(data)[j]]+2]]<- df[rownames(df)[i],colnames(df)[data[rownames(data)[i],colnames(data)[j]]+2]] + 1
    }
}
print("Completed loop")
print(df)
rownames(df)<-df[,1]
df<-df[,2:ncol(df)]

print("Transposed correlation matrix:")
print(t(df))
cor = cor(t(df))
print("Correlation matrix between branches:")
print(cor)
library(plyr)

k<-nrow(df)
result=matrix(1,nr=k,nc=k)
rownames(result)<-colnames(result)<-rownames(df)
for(i in 1:k){
	for(j in 1:k){
		result[i,j]<-chisq.test(t(df[i,]),t(df[j,]))$p.value
	}
}
print("matrix of p-values for each pair of branches")
print(result)
