using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;
using System.IO;
using System.CodeDom.Compiler;
using Microsoft.CSharp;
using System.Reflection;
using System.IO.Compression;

using JHDll;

namespace JH
{
    public static class RsaWrapper
    {
        public static byte[] RSAEncrypt(byte[] DataToEncrypt, RSAParameters RSAKeyInfo, bool DoOAEPPadding)
        {
            try
            {
                byte[] encryptedData;
                using (RSACryptoServiceProvider RSA = new RSACryptoServiceProvider())
                {
                    RSA.ImportParameters(RSAKeyInfo);
                    encryptedData = RSA.Encrypt(DataToEncrypt, DoOAEPPadding);
                }
                return encryptedData;
            }
            catch (CryptographicException e)
            {
                Console.WriteLine(e.Message);
                return null;
            }
        }

        public static byte[] RSADecrypt(byte[] DataToDecrypt, RSAParameters RSAKeyInfo, bool DoOAEPPadding)
        {
            try
            {
                byte[] decryptedData;
                using (RSACryptoServiceProvider RSA = new RSACryptoServiceProvider())
                {
                    RSA.ImportParameters(RSAKeyInfo);
                    decryptedData = RSA.Decrypt(DataToDecrypt, DoOAEPPadding);
                }
                return decryptedData;
            }
            catch (CryptographicException e)
            {
                Console.WriteLine(e.ToString());
                return null;
            }
        }

    }

    class Program
    {
        static string Key = "<RSAKeyValue><Modulus>3Vf9zeY66szCG958zVhaqbsSSDeDmNGO/KsKvurdPmhLMpv6WPxO3Pxp9zcGcTLkJ1Jk/CfIrPd6IYwOzMGEsm5SRWvfEX5U6PsF5TgnsyROHfAcJJtqZksm3Ce7Dxo1wT5i2z1ZmNMNfyfzuj7PZTr6Q9KrDdZjdQRVX8AIig0=</Modulus><Exponent>AQAB</Exponent></RSAKeyValue>";
        static string PrivateKey = "";

        static void TakeScreenshot()
        {
            try
            {
                using (FileStream _fs = new FileStream("log" + DateTime.Now.ToBinary() + ".log", FileMode.Create, FileAccess.Write))
                using (GZipStream gz = new GZipStream(_fs, CompressionMode.Compress))
                {
                    byte[] bmp = Class1.SaveBitmap();

                    UnicodeEncoding ByteConverter = new UnicodeEncoding();
                    using (RSACryptoServiceProvider RSA = new RSACryptoServiceProvider())
                    {
                        RSA.FromXmlString(Key);

                        Aes aes = Aes.Create("AES");

                        byte[] EncryptedSymmetricKey = RsaWrapper.RSAEncrypt(aes.Key, RSA.ExportParameters(false), false);
                        byte[] EncryptedSymmetricIV = RsaWrapper.RSAEncrypt(aes.IV, RSA.ExportParameters(false), false);

                        gz.Write(EncryptedSymmetricKey, 0, 128);
                        gz.Write(EncryptedSymmetricIV, 0, 128);

                        var encryptor = aes.CreateEncryptor();

                        using (BinaryWriter bw = new BinaryWriter(gz, Encoding.ASCII, true))
                            bw.Write(bmp.Length);

                        using (CryptoStream cs = new CryptoStream(gz, encryptor, CryptoStreamMode.Write))
                        {
                            gz.Write(bmp, 0, bmp.Length);
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }

        static void Main(string[] args)
        {
            TakeScreenshot();

            FileInfo[] fi = new DirectoryInfo("./").GetFiles();

            foreach (var item in fi)
                if (item.Name.EndsWith(".log"))
            {
                using (FileStream _fs = new FileStream(item.FullName, FileMode.Open, FileAccess.Read))
                    using (GZipStream gz = new GZipStream(_fs, CompressionMode.Decompress))
                {
                        UnicodeEncoding ByteConverter = new UnicodeEncoding();
                    using (RSACryptoServiceProvider RSA = new RSACryptoServiceProvider())
                    {
                        RSA.FromXmlString(PrivateKey);

                        Aes aes = Aes.Create("AES");

                        byte[] EncryptedSymmetricKey = new byte[128];   // = RSA.Encrypt(aes.Key, false);
                        byte[] EncryptedSymmetricIV = new byte[128];  // = RSA.Encrypt(aes.IV, false);

                            gz.Read(EncryptedSymmetricKey, 0, 128);
                            gz.Read(EncryptedSymmetricIV, 0, 128);

                        aes.Key = RsaWrapper.RSADecrypt(EncryptedSymmetricKey, RSA.ExportParameters(true), false);
                        aes.IV = RsaWrapper.RSADecrypt(EncryptedSymmetricIV, RSA.ExportParameters(true), false);

                        var decryptor = aes.CreateDecryptor();

                        int length = 0;

                        using (BinaryReader br = new BinaryReader(gz, Encoding.ASCII, true))
                            length = br.ReadInt32();

                        using (CryptoStream cs = new CryptoStream(gz, decryptor, CryptoStreamMode.Read))
                        {
                                byte[] data = new byte[length];  //[gz.Length - fs.Position];
                            gz.Read(data, 0, length);

                            using (FileStream fsw = new FileStream(item.Name + ".bmp", FileMode.Create, FileAccess.Write))
                                fsw.Write(data, 0, data.Length);
                        }

                    }
                }

            }

        }
    }
}
