#include "encry.h"
#include <iostream>
#include<string.h>


//#ifdef __cplusplus
//extern "C" {
//#endif 
//#include<openssl/applink.c>
//#ifdef __cplusplus
//}
//#endif

static char* cipher__bin2tobase64(const char* in, int in_size, char* (&out), int out_szie)
{
	int buf_size = out_szie;
	BUF_MEM* memptr = nullptr;
	BIO* base64 = BIO_new(BIO_f_base64());  // ����һ��base64���� BIO����
	BIO* mem = BIO_new(BIO_s_mem());// ����һ���ڴ���� BIO����

	BIO_push(base64, mem);  // ��֯bio��(�߼���ϵ: �ȱ���,�ٴ���ڴ�BIO)
	BIO_write(base64, in, in_size + 1);    // ����
	BIO_flush(base64);
	BIO_get_mem_ptr(mem, &memptr);  // ��ȡbio�ڴ�����б�������ݵ�����
	if (out != nullptr && out_szie < memptr->length)
	{// ��������С< ���ݴ�Сʱ
		BIO_free_all(base64);
		return nullptr;
	}
	if (out == nullptr)
	{
		out = new char[memptr->length + 2];
		memset(out + memptr->length, 0, 2);
		buf_size = memptr->length;
	}
	memcpy(out, memptr->data, buf_size);
	BIO_free_all(base64);		// �Զ��ͷű�����
	return out;
}

static int cipher__base64tobin2(const char* in, int in_size, char* (&out), int out_szie)
{
	int outbuf_isNull = (out == nullptr ? true : false);
	int dec_size = in_size / 4 * 3;
	// ���������ݻ��߻������ĳ���,˭����˭
	BIO* base64 = nullptr;
	BIO* mem = nullptr;

	if (out != nullptr && dec_size > out_szie)
		return 0;
	if (out == nullptr)
	{   // �����մ洢���ݵ�ָ��Ϊnull���߻�������С<���ݴ�Сʱ
		out = new char[dec_size + 2];
		memset(out, 0, dec_size + 2);
	}
	base64 = BIO_new(BIO_f_base64());
	mem = BIO_new_mem_buf(in, in_size);
	BIO_push(base64, mem);
	dec_size = BIO_read(base64, out, dec_size);
	if (dec_size < 1)
	{   // ����ֵΪ�������ݸ���,�жϽ����Ƿ�ɹ�
		if (!outbuf_isNull)
			delete[] out;
		BIO_free_all(base64);
		return 0;
	}
	BIO_flush(base64);
	BIO_free_all(base64);
	return dec_size;
}

CEncryAES::CEncryAES(const  char* userKey, const int bitSize, const   char* ivec)
{
	this->InitAesKey(userKey, bitSize, ivec);
	return;
}

bool CEncryAES::InitAesKey(const  char* userKey, const int bitSize, const  char* ivec)
{
	this->m_userKey = (char*)userKey;
	this->m_bitSize = bitSize;
	memcpy(&this->ivec, ivec, AES_BLOCK_SIZE);
	AES_set_encrypt_key((unsigned char*)userKey, bitSize, &this->m_enc_AESKey);
	AES_set_decrypt_key((unsigned char*)userKey, bitSize, &this->m_dec_AESKey);
	return true;
}



std::string  CEncryAES::AesCbcEncrypt(std::string enc_data)
{
	//unsigned int data_Len = wcslen((const wchar_t*)enc_data) * 2;
	unsigned int data_Len = enc_data.size();
	char* encData = nullptr, *encBioData = nullptr;
	unsigned char ivec[AES_BLOCK_SIZE] = { 0 };
	std::string encStrData;
	if ((data_Len % AES_BLOCK_SIZE) != 0)
		data_Len = (data_Len / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
	encData = new char[data_Len + 2];
	memset(encData, 0, data_Len + 2);
	memcpy(ivec, this->ivec, AES_BLOCK_SIZE);
	
	// ���ܺ���
	AES_cbc_encrypt((unsigned char*)enc_data.data(), (unsigned char*)encData, data_Len, &this->m_enc_AESKey,
		ivec, AES_ENCRYPT);

	// ���ܺ�ת��ΪBase64
	cipher__bin2tobase64(encData, data_Len, encBioData, 0);
	encStrData = encBioData;
	delete[] encData;
	delete[] encBioData;
	return encStrData;
}

std::string  CEncryAES::AesCbcDecrypt(std::string dec_data)
{
	//unsigned int data_Len = wcslen((const wchar_t*)dec_data) * 2;
	unsigned int data_Len = 0;
	char* decData = nullptr, *decBioData = nullptr;
	unsigned char ivec[AES_BLOCK_SIZE] = { 0 };
	std::string encStrData;

	data_Len= cipher__base64tobin2(dec_data.data(), dec_data.size(), decBioData, 0);
	if ((data_Len % AES_BLOCK_SIZE) != 0)
		data_Len = (data_Len / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
	
	decData = new char[data_Len + 2];
	memset(decData, 0, data_Len + 2);
	memcpy(ivec, this->ivec, AES_BLOCK_SIZE);
	AES_cbc_encrypt((unsigned char*)decBioData, (unsigned char*)decData, data_Len, &this->m_dec_AESKey,
		ivec, AES_DECRYPT);

	encStrData = decData;
	delete[] decBioData;
	delete[] decData;
	return encStrData;
}


CEncryRSA::CEncryRSA()
{
	this->m_rsa = RSA_new();
}

CEncryRSA::CEncryRSA(int bits, const char* pubFilePath, const char* privFilePath)
{
	this->m_rsa = RSA_new();
	this->RsaGenerateRsaKey(bits, pubFilePath, privFilePath);
	return;
}

CEncryRSA::CEncryRSA(const char* pubFilePath, const char* privFilePath)
{
	this->m_rsa = RSA_new();
	this->RsaOpenKey(pubFilePath, privFilePath);
	return;
}

CEncryRSA::~CEncryRSA()
{
	if (this->m_rsa)
		RSA_free(this->m_rsa);
}

// ���Ѵ�ĵĹ�˽Կ�ļ�
bool CEncryRSA::RsaOpenKey(const char* pubFilePath, const char* privFilePath)
{
	BIO* publie_File = nullptr;
	BIO* private_File = nullptr;
	bool status = false;
	if (pubFilePath)
	{	// ��Կ�ļ�·������
		publie_File = BIO_new_file(pubFilePath, "rb");
		if(PEM_read_bio_RSAPublicKey(publie_File, &this->m_rsa, NULL, NULL))
			status = true;
		BIO_free(publie_File);
		
	}
	if (privFilePath)
	{	// ˽Կ�ļ�·������
		private_File = BIO_new_file(privFilePath, "rb");
		if(PEM_read_bio_RSAPrivateKey(private_File, &this->m_rsa, NULL, NULL))
			status = true;
		BIO_free(private_File);
	}
	return status;
}

// �����µ���Կ��,���·��Ϊ���򲻱����ڴ����ļ���(��Կ�Գ���,��Կ·��, ˽Կ·��);
void CEncryRSA::RsaGenerateRsaKey(int bits, const char* pubFilePath, const char* privFilePath)
{
	BIGNUM* bigNum = BN_new();
	BIO* publicFile = nullptr;
	BIO* privateFile = nullptr;

	int i = BN_set_word(bigNum, 123456789);					// ��ʼ��������Կʱ�����������
	RSA_generate_key_ex(this->m_rsa, bits, bigNum, NULL);	// ������Կ��
	if (pubFilePath)
	{	// ��Կ���ñ���·��
		publicFile = BIO_new_file(pubFilePath, "wb");			// ��bio�ķ�ʽ�򿪹�Կ�ļ�,û�лᱻ����
		PEM_write_bio_RSAPublicKey(publicFile, this->m_rsa);	// ����д��
		BIO_flush(publicFile);									// ˢ�¹�Կ�ļ���������
		BIO_free(publicFile);									// �ͷ��ļ���Դ
	}
	if (privFilePath)
	{	// ˽Կ���ñ���·��
		privateFile = BIO_new_file(privFilePath, "wb");			// ��bio�ķ�ʽ��˽Կ�ļ�,û�лᱻ����
		PEM_write_bio_RSAPrivateKey(privateFile, this->m_rsa, NULL, NULL, 0, NULL, NULL); // ����д��
		BIO_flush(privateFile);									//  ˢ��˽Կ�ļ���������
		BIO_free(privateFile);									// �ͷ��ļ���Դ
	}
	BN_free(bigNum);
}

// RSA˽Կ����
std::string  CEncryRSA::RsaPrivateEncrypt(std::string from)
{
	int data_Len = from.size();					// ����Ҫ���ܵ������ܳ���
	int buf_size = 0;							// ���ݼ��ܺ�����������һ����Ҫ�ĳ���
	int rsa_size = RSA_size(this->m_rsa);		// ����ÿ�μ��ܿ�ĳ���(����������� )
	int rsa_encdata_size = rsa_size - RSA_PKCS1_PADDING_SIZE;		// ����ÿ��ɼ������ݳ���

	int orgdata_offset = 0;			// ÿ�μ��ܺ�� ԭʼ����δ����ƫ��
	int encdata_offset = 0;			// �������ݳ���ƫ��

	const char* form_data = from.data();
	char* data_buf = nullptr, *enc_data = nullptr;
	std::string enc_strdata;

	// �������ݼ��ܺ�����Ҫ�ڴ���ܴ�С
	if (data_Len > rsa_encdata_size)
	{	// ԭʼ���ݳ��� > ���οɼ������ݳ���
		// ((ԭʼ���ݳ���/ÿ�οɼ������ݳ���+1 == һ��Ҫ���ܶ��ٿ� ) *ÿ��Ĵ�С) +2
		buf_size = (data_Len / rsa_encdata_size + 1) * rsa_size + 2;
	}
	else
		buf_size = rsa_size + 2;

	data_buf = new char[buf_size];
	memset(data_buf, 0, buf_size);
	while (true)
	{
		int i = 0;
		if (data_Len - orgdata_offset > rsa_encdata_size)
		{	// ʣ��δ�������� > ÿ�οɼ������ݳ���
			i = RSA_private_encrypt(rsa_encdata_size, (const unsigned char*)form_data + orgdata_offset,
				(unsigned char*)data_buf + encdata_offset, this->m_rsa, RSA_PKCS1_PADDING);
			orgdata_offset += rsa_encdata_size;
			encdata_offset += rsa_size;
			continue;
		}
		i = RSA_private_encrypt(data_Len - orgdata_offset, (const unsigned char*)form_data + orgdata_offset,
			(unsigned char*)data_buf + encdata_offset, this->m_rsa, RSA_PKCS1_PADDING);
		orgdata_offset += (data_Len - orgdata_offset);
		encdata_offset += rsa_size;
		break;
	}
	cipher__bin2tobase64(data_buf, encdata_offset, enc_data, 0);
	enc_strdata = enc_data;
	delete[] enc_data;
	delete[] data_buf;
	return enc_strdata;
}

// RSA��Կ����
std::string  CEncryRSA::RsaPublicEncrypt(std::string  from)
{
	int data_Len = from.size();					// ����Ҫ���ܵ������ܳ���
	int rsa_size = RSA_size(this->m_rsa);		// ����ÿ�μ��ܿ�ĳ���(����������� )
	int rsa_encdata_size = rsa_size - RSA_PKCS1_PADDING_SIZE;		// ����ÿ��ɼ������ݳ���
	int buf_size = 0;							// ���ݼ��ܺ����������� һ����Ҫ�ĳ���
	int addre_offset = 0;						//ÿ�μ��ܺ�� ԭʼ����δ����ƫ�ơ�
	int data_offset = 0;						// �Ѽ������ݳ���ƫ��

	const char* from_data = from.data();
	char* data_buf = nullptr, *enc_data = nullptr;
	std::string enc_strdata;

	if (data_Len > rsa_size)
		buf_size = (data_Len / rsa_encdata_size + 1) * rsa_size + 2;
	else
		buf_size = rsa_size + 2;

	data_buf = new char[buf_size];
	memset(data_buf, 0, buf_size);
	while (true)
	{
		if (data_Len - data_offset > rsa_encdata_size)
		{
			RSA_public_encrypt(rsa_encdata_size, (const unsigned char*)from_data + data_offset,
				(unsigned char*)data_buf + addre_offset, this->m_rsa, RSA_PKCS1_PADDING);
			addre_offset += rsa_size;
			data_offset += rsa_encdata_size;
			continue;
		}
		RSA_public_encrypt(data_Len - data_offset, (const unsigned char*)from_data + data_offset,
			(unsigned char*)data_buf + addre_offset, this->m_rsa, RSA_PKCS1_PADDING);
		addre_offset += rsa_size;
		data_offset += data_Len - data_offset;
		break;
	}
	cipher__bin2tobase64(data_buf, addre_offset, enc_data, 0);
	enc_strdata = enc_data;
	delete[] enc_data;
	delete[] data_buf;
	return enc_strdata;
}

// RSA˽Կ����
std::string CEncryRSA::RsaPrivateDecrypt(std::string from)
{
	int rsa_size = RSA_size(this->m_rsa);
	int buf_size = 0; int data_Len = 0;
	int rsa_decdata_size = rsa_size - RSA_PKCS1_PADDING_SIZE;	// ÿ�οɽ������ݵĳ���
	int decdata_offset = 0;	// ���������Ѿ����ܵ����ݳ���
	int encdata_offset = 0;	// �������ݺ�����ݳ���(rsa_size - RSA_PKCS1_PADDING_SIZE*�ڼ��ֽ���)
	char* data_buf = nullptr, *dec_data = nullptr;
	std::string dec_strdata;

	data_Len = cipher__base64tobin2(from.data(), from.size(), data_buf, 0);
	if (data_Len > rsa_size)
		buf_size = (data_Len / rsa_decdata_size + 1) * rsa_size + 2;
	else
		buf_size = rsa_decdata_size + 2;

	dec_data = new char[buf_size];
	memset(dec_data, 0, buf_size);
	while (true)
	{
		if (data_Len - decdata_offset > rsa_size)
		{
			RSA_private_decrypt(rsa_size, (const unsigned char*)data_buf + decdata_offset,
				(unsigned char*)dec_data + encdata_offset, this->m_rsa, RSA_PKCS1_PADDING);
			decdata_offset += rsa_size;
			encdata_offset += (rsa_size - 11);
			continue;
		}
		RSA_private_decrypt(data_Len - decdata_offset, (const unsigned char*)data_buf + decdata_offset,
			(unsigned char*)dec_data + encdata_offset, this->m_rsa, RSA_PKCS1_PADDING);
		break;
	}
	dec_strdata = dec_data;
	delete[] data_buf;
	delete[] dec_data;
	return dec_strdata;
}

// RSA��Կ����
std::string CEncryRSA::RsaPublicDecrypt(std::string from)
{
	int rsa_size = RSA_size(this->m_rsa);
	int decdata_offset = 0;	// ���������Ѿ����ܵ����ݳ���
	int encdata_offset = 0;	// �������ݺ�����ݳ���(rsa_size - RSA_PKCS1_PADDING_SIZE*�ڼ��ֽ���)
	int rsa_decdata_size = rsa_size - RSA_PKCS1_PADDING_SIZE;	// ÿ�ν��ܳ��������ݵĳ���


	int data_Len = 0, buf_size = 0;
	char* data_buf = nullptr, *dec_data = nullptr;
	std::string dec_strdata;

	data_Len = cipher__base64tobin2(from.data(), from.size(), data_buf, 0);
	if (data_Len > rsa_size)
		buf_size = (data_Len / rsa_decdata_size + 1) * rsa_size + 2;
	else
		buf_size = rsa_decdata_size + 2;

	dec_data = new char[buf_size];
	memset(dec_data, 0, buf_size);

	while (true)
	{
		if (data_Len - decdata_offset > rsa_size)
		{
			RSA_public_decrypt(rsa_size, (const unsigned char*)data_buf + decdata_offset,
				(unsigned char*)dec_data + encdata_offset, this->m_rsa, RSA_PKCS1_PADDING);
			decdata_offset += rsa_size;
			encdata_offset += rsa_decdata_size;
			continue;
		}
		RSA_public_decrypt(data_Len - decdata_offset, (const unsigned char*)data_buf + decdata_offset,
			(unsigned char*)dec_data + encdata_offset, this->m_rsa, RSA_PKCS1_PADDING);
		break;
	}
	dec_strdata = dec_data;
	delete[] data_buf;
	delete[] dec_data;
	return dec_strdata;
}

// ǩ������, ����from�Ĺ�ϣ������ϣʹ��˽Կ����,����˽Կ���ܺ�Ĺ�ϣ(type������ʾʹ�õ�hash�㷨,��δʹ��)
std::string CEncryRSA::RSASign(int type, const std::string from)
{
	//��ϣ�㷨дsha224
	CHashSha224 sha224;
	unsigned int hash_encdata_size = SHA224_DIGEST_LENGTH + 2;
	std::string hash_enc_strdata;
	std::string rsa_enc_strdata;
	
	hash_enc_strdata = sha224.GetDataHash(from);
	if (hash_enc_strdata.size() <= 0)
		return rsa_enc_strdata;
	rsa_enc_strdata = this->RsaPrivateEncrypt(hash_enc_strdata);
	if (rsa_enc_strdata.size() <= 0)
		std::cout << "ǩ������ʧ��" << std::endl;
	return rsa_enc_strdata;
}

//  RSAǩ����֤
bool  CEncryRSA::RSAVerify(std::string& org_data, std::string& sign_data)
{
	int status = 0;
	CHashSha224 sha224;
	std::string hash_enc_strdata;
	std::string sign_rsadec_strdata;

	hash_enc_strdata = sha224.GetDataHash(org_data.data());
	sign_rsadec_strdata = this->RsaPublicDecrypt(sign_data);
	status = memcmp(hash_enc_strdata.data(), sign_rsadec_strdata.data(), SHA224_DIGEST_LENGTH);
	return status== 0? true:false;
}

// =============================================== HashSha
CHashSha224::CHashSha224()
{
	this->InitSha224();
	return;
}

void CHashSha224::InitSha224()
{
	SHA224_Init(&this->m_sha224);
}

void CHashSha224::addData(std::string data)
{
	int data_len = data.size();
	SHA224_Update(&this->m_sha224, data.data(), data_len);
	return;
}

std::string CHashSha224::GetDataHash()
{
	char* buf = new char[SHA224_DIGEST_LENGTH + 2];
	std::string data;
	memset(buf, 0, SHA224_DIGEST_LENGTH + 2);
	SHA224_Final((unsigned char*)buf, &this->m_sha224);
	data = buf;
	delete[] buf;
	return data;
}

std::string CHashSha224::GetDataHash(const std::string from)
{
	char* hash_data = new char[SHA224_DIGEST_LENGTH + 2];
	std::string hash_strdata;

	memset(hash_data, 0, SHA224_DIGEST_LENGTH + 2);
	if (SHA224((unsigned char*)from.data(), from.size(), (unsigned char*)hash_data) != NULL)
		hash_strdata = hash_data;
	delete[] hash_data;
	return hash_strdata;
}
