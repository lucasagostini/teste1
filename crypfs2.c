#ifndef _CRYFS_H_
#define _CRYFS_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define SUCESSO 1
#define FALHA 0

#define LEITURA 0
#define ESCRITA 1
#define LEITURAESCRITA 2


#define BLOCO 4096

/** Descritor de um arquivo
 *  Timestamps devem ser obtidos com a funcao time() da stdlib
 */
typedef struct descritor{
	char nome[256];
	time_t criacao; /* timestamp de quando foi criado */
	time_t modificacao; /* timestamp da ultima modificao */
	time_t ultimo_acesso; /* timestamp do ultimo acesso */
	uint32_t tamanho;
} descritor_t;

/** Tabela de arquivos abertos.
 */
typedef struct arquivo_aberto{
	descritor_t * arquivo;
	uint32_t acesso; /* pode ser LEITURA, ESCRITA ou LEITURAESCRITA */
	uint32_t posicao; /* seek atual no arquivo */
} arquivo_aberto_t;

/** Descritor de um sistema de arquivos criptografados
 */
typedef struct cry_descritor{
	FILE * arquivo_host;
	descritor_t descritores[256]; /* devem ser lidos do arquivo host e mantidos sincronizados */
	arquivo_aberto_t abertos[256];  /* tabela de arquivos abertos */
} cry_desc_t;

typedef int indice_arquivo_t;
cry_desc_t* cry_desc2;	//cria um descritor de arquivos global

int procura_nome (cry_desc_t* desc, char* nome) {
	int status=-1;
	for (int i = 0; i < 256; ++i) {

		if(status == -1 && desc->descritores[i].nome[0] == 0) {
			status=i;
		}

		if(!strcmp(desc->descritores[i].nome, nome)) {

			return i;

		}

	}
	return status;
}



int initfs(char ** arquivo, int blocos)
	{	/** Inicializa o arquivo no sistema de arquivos hospedeiro.
		*
		*  Deve ser chamada apenas uma vez para um determinado sistema de arquivos.
		*  Caso o nome de arquivo já exista, deve retornar erro. OK
		*  Também deve retornar erro caso o número de blocos não seja o suficiente
		*  para armazenar os metadados pelo menos. OK
		*
		*  @param arquivo nome do arquivo no sistema de arquivos hospedeiro
		*  @param blocos número de blocos do arquivo
		*  @return SUCCESSO ou FALHA
		*/
		FILE *fp;
		if (blocos<19) {
			return FALHA;
		}

		if(access(arquivo, F_OK) != -1) {
			return FALHA;
		}

		fp = fopen(arquivo,"w+");
		for(i = 0; i < (blocos * 4096); i++) { // blocos*BLOCO = tamanho em bytes
			fputc(ch, fp); // escreve 1 byte de zeros
		}

		rewind(fp);
		descritor_t* descritores = (descritor_t*) malloc(sizeof(descritor_t) * 256);
		arquivo_aberto_t* arquivos_abertos = (arquivo_aberto_t*) malloc(sizeof(arquivo_aberto_t) * 256);


		for(i = 0; i < 256; i++) {


			descritores[i].nome[0] = 0;
			descritores[i].tempo = 0;
			descritores[i].indices = 0;
			descritores[i].tamanho = 0;

			arquivos_abertos[i].arquivo = NULL;
			arquivos_abertos[i].acesso = -1;

		}

		fwrite(descritores, sizeof(descritor_t), 256, fp);
		fwrite(arquivos_abertos, sizeof(arquivo_aberto_t), 256, fp);
		return SUCCESSO;
	}


cry_desc_t * cry_openfs(char ** arquivo)
	{/** Abre um sistema de arquivos.
	 *
	 *  @param nome do arquivo no sistema de arquivos hospedeiro
	 *  @return ponteiro para descritor de sistema de arquivos ou FALHA
	 */
	 	FILE *fp;
	 	cry_desc_t* descritor = (cry_desc_t*) malloc(sizeof(cry_desc_t));
		if(access(arquivo, F_OK) == -1) {
    		return FALHA;
   		}

		fp = fopen(arquivo, "r+");
		descritor->arquivo_host = fp;

	    fread(descritor->descritores, sizeof(descritor_t), 256, fp);
	    fread(descritor->abertos, sizeof(arquivo_aberto_t), 256, fp);
	    cry_desc2 = (cry_desc_t*) malloc(sizeof(cry_desc_t));
	    cry_desc2 = descritor;
}



indice_arquivo_t cry_open(cry_desc_t *cry_desc, char * nome,  int acesso, char deslocamento)
	{/** Abre um arquivo criptografado.
 *
 *
 * @param cry_desc o descritor do sistema de arquivos criptografado
 * @param nome o arquivo a ser aberto
 * @param acesso LEITURA, ESCRITA ou LEITURAESCRITA
 * @param chave deslocamento a ser usado nos acessos
 * @return índice do arquivo aberto, FALHA se não abrir
 */

}
int cry_close(indice_arquivo_t arquivo)
	{/** Fecha um arquivo criptografado.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO OU FALHA
 */
	 if(cry_desc2->abertos[arquivo].arquivo != NULL ){
			cry_desc2->abertos[arquivo].arquivo = NULL;
			cry_desc2->abertos[arquivo].acesso=0;
			cry_desc2->abertos[arquivo].posicao=0;
			return SUCCESSO;
		}
		return FALHA;
}
uint32_t cry_read(indice_arquivo_t arquivo, uint32_t tamanho, char *buffer)
	{/** Lê bytes de um arquivo criptografado aberto.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param tamanho número de bytes a serem lidos
 * @param buffer ponteiro para buffer onde serão armazenados os bytes lidos
 * @return número de bytes lidos
 */

}
int cry_write(indice_arquivo_t arquivo, uint32_t tamanho, char *buffer)
	{/** Escreve bytes em um arquivo criptografado aberto.
 *
 * A escrita é sempre realizada no modo append, ou seja, no fim do arquivo.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param tamanho número de bytes a serem escritos
 * @param buffer ponteiro para conteúdo a ser escrito
 * @return SUCESSO ou FALHA
 */

}
int cry_delete(indice_arquivo_t arquivo)
	{/** Apaga um arquivo e o fecha.
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return SUCESSO ou FALHA
 */

}
int cry_seek(indice_arquivo_t arquivo, uint32_t seek)
	{/** Modifica a posição atual de leitura ou escrita do arquivo
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @param seek deslocamento em relação ao início do arquivo
 * @return SUCESSO ou FALHA
 */	
 	if(abertos[arquivo]==NULL)
 		return FALHA;
 	abertos[arquivo].seek=seek;
 	return SUCESSO;
}
time_t cry_creation(indice_arquivo_t arquivo)
	{/** Retorna o tempo em que o arquivo foi criado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
 	return abertos[arquivo].arquivo.criacao;
}
time_t cry_accessed(indice_arquivo_t arquivo)
	{/** Retorna o tempo em que o arquivo foi acessado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
 	return abertos[arquivo].arquivo.acessado;
}
time_t cry_last_modified(indice_arquivo_t arquivo)
	{/** Retorna o tempo em que o arquivo foi modificado
 *
 * @param arquivo índice para a tabela de arquivos abertos
 * @return tempo
 */
 	return abertos[arquivo].arquivo.modificado;
}
#endif
