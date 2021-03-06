# Huffman-文件压缩



---
利用Huffman编码思想进行文件压缩和解压缩。主要原理是通过Huffman树根据字符出现的次数重新存储字符，以字符出现的频率为权值，把出现频率小的放在远处，频率大放在离根更近的位置。Huffman算法的优势就在于对于权值的处理，因此，如果被压缩的字符出现的次数差别很小的时候，它的效率会大打折扣。<br/>

此文件压缩主要分为以下两个部分：<br/>
**压缩：**<br/>

 1. 统计字符出现的次数<br/>
 文件的底层都是以字符的形式来存储的，我们使用一个256的数组来统计字符出现的次数<br/>
 2. 构建Huffman树<br/>
以字符出现的次数为权值，采用Huffman算法。具体构建过程为：将集合中权值最小的两个元素拿出来构建成一棵树，以他们的权值之和作为父节点并且重新插入此集合，不断重复直到构成一棵树。这里为了增加寻找最小两棵树的效率，利用最小堆来保存上面的元素集合。<br/>
 3. 得到Huffman编码<br/>
从根结点出发，向左走标记为0，向右走标记为1，每到达一个字符节点，就将它所对应的编码保存起来</cr>
 4.  将Huffman编码写入文件<br/>
将每8位编码构成的一个字符写入文件中，后边不够8位的补0<br/>
 5.  编写配置文件<br/>
为了解压正确，必须在压缩文件里面标记压缩类型以及被压缩字符和它出现的次数，为了方便解析，以行为单位。<br/>

**解压：**<br/>
 1. 读取配置文件<br/>
以行为单位进行读取，使用atoi()函数将字符转化为数字。<br/>
 2. 还原Huffman树<br/>
和压缩时的构建方法一样。<br/>
 3. 解压缩<br/>
根据压缩文件里面还原出来的字符，只要遇到一个叶子节点，就还原出一个字符。但要注意最后几位编码可能是补上去的，因此要根据字符的数量来还原，Huffman树根结点的权值就是字符的数量。<br/>
