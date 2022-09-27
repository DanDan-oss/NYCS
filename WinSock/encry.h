#ifndef ENCRY_H
#define ENCRY_H

#include <iostream>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>


/* start��������ָ������ָ��nullptr���������Զ������ڴ�,ע���Լ��ͷ�*/
// ����������תbase64���л�
static char* cipher__bin2tobase64(const char* in, int in_size, char* (&out) , int out_szie);
// base64����ת������ �����л�
static int cipher__base64tobin2(const char* in, int in_size, char* (&out), int out_szie);
/* end*/

class CEncryAES
{
public:
    CEncryAES(const char* userKey, const int bitSize = 128, 
        const char* ivec = "csaersacfkacseacv");
	bool InitAesKey(const  char* userKey, const int bitSize, const char* ivec);

    // ����
    std::string AesCbcEncrypt(std::string enc_data);

    // ����
    std::string AesCbcDecrypt(std::string dec_data);
private:
    CEncryAES() { }
	std::string m_userKey;
	int m_bitSize = 0;
	char ivec[16] = {0};
	AES_KEY m_dec_AESKey = {0};
	AES_KEY m_enc_AESKey = {0};
};

class CEncryRSA
{
public:
    CEncryRSA();
    // Ϊ�˴���һ������Կ��
    CEncryRSA(int bits, const char* pubFilePath, const char* privFilePath); 

    // ��һ������Կ��
    CEncryRSA(const char* pubFilePath, const char* privFilePath);

    ~CEncryRSA();
    // ���Ѵ�ĵĹ�˽Կ�ļ�
    bool RsaOpenKey(const char* pubFilePath, const char* privFilePath);

    //�����µ���Կ�Ա����ļ�,���·��Ϊ���򲻱����ڴ���(��Կ�Գ���,��Կ·��, ˽Կ·��);
    void RsaGenerateRsaKey(int bits, const char* pubFilePath = nullptr, const char* privFilePath = nullptr);

    // ��Կ����
    std::string  RsaPublicEncrypt(std::string  from);

    // ˽Կ����
    std::string RsaPrivateDecrypt(std::string from);

    // ˽Կ����
    std::string RsaPrivateEncrypt(std::string from);

    // ��Կ����
    std::string RsaPublicDecrypt(std::string from); 

    // ǩ������, type��ʾǩ��ʹ�õļ����㷨,��ûʹ��
    std::string RSASign(int type, const std::string from);   
    // ǩ����֤     
    bool RSAVerify(std::string& org_data, std::string& sign_data);

private:
    RSA* m_rsa = nullptr;
};

class CHashSha224
{
public:
    CHashSha224();
    void InitSha224();
    void addData(std::string data);
    std::string GetDataHash();
    std::string GetDataHash(const std::string from);
private:
    SHA256_CTX m_sha224 = {0};
};

#endif // !ENCRY_H