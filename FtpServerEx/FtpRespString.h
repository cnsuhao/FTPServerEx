#ifndef __H_FTPRESPSTRING__
#define __H_FTPRESPSTRING__

/* const string */
/*
    2开头－－成功 
    3开头－－权限问题 
    4开头－－文件问题 
    5开头－－服务器问题
*/
const char *const RESP_120 = "120 Service ready in N minutes.\r\n";
const char *const RESP_120EX = "120 Service ready in %d minutes.\r\n";
const char *const RESP_125 = "125 Data connection already open; transfer starting.\r\n";
const char *const RESP_150 = "150 File status okay; about to open data connection.\r\n";
const char *const RESP_200 = "200 Commond OKey\r\n";
const char *const RESP_200EX = "200 %s\r\n";
const char *const RESP_202 = "202 Command not implemented, superfluous at this site.\r\n";
const char *const RESP_211 = "211 System status, or system help reply.\r\n";
const char *const RESP_212 = "212 Directory status.\r\n";
const char *const RESP_213TIME = "213 %s.\r\n";
const char *const RESP_213EX = "213 Size of file is '%s'.\r\n";
const char *const RESP_214 = "214 Help message.On how to use the server or the meaning of a particular non-standard command. This reply is useful only to the human user.\r\n";
const char *const RESP_214EX = "214 %s.\r\n";
const char *const RESP_215 = "215 UNIX Type: L8.\r\n";
const char *const RESP_215EX = "215 %s\r\n";
const char *const RESP_220 = "220 Service ready for new user.\r\n";
const char *const RESP_220EX = "220 %s.\r\n";
const char *const RESP_221 = "221 Service closing control connection.\r\n";
const char *const RESP_225 = "225 Data connection open; no transfer in progress.\r\n";
const char *const RESP_226 = "226 Transfer complete.\r\n";
const char *const RESP_227EX = "227 Entering Passive Mode (%s,%d,%d).\r\n";
const char *const RESP_230 = "230 User successfully logged in.\r\n";
const char *const RESP_230EX = "230 User %s successfully logged in.\r\n";
const char *const RESP_250 = "250 Requested file action okay, completed.\r\n";
const char *const RESP_250EX = "250 %s.\r\n";
const char *const RESP_257EX = "257 '%s' is current directory.\r\n";
const char *const RESP_331 = "331 User name okay, need password.\r\n";
const char *const RESP_331EX = "331 User %s okay, need password.\r\n";
const char *const RESP_332 = "332 Need account for login.\r\n";
const char *const RESP_333 = "333 already logined.\r\n";
const char *const RESP_350 = "350 Requested file action pending further information.\r\n";
const char *const RESP_421 = "421 Service not available, closing control connection.This may be a reply to any command if the service knows it must shut down.\r\n";
const char *const RESP_425 = "425 Can't open data connection.\r\n";
const char *const RESP_426 = "426 Connection closed; transfer aborted.\r\n";
const char *const RESP_450 = "450 Requested file action not taken.\r\n";
const char *const RESP_451 = "451 Requested action aborted. Local error in processing.\r\n";
const char *const RESP_452 = "452 Requested action not taken. Insufficient storage space in system.File unavailable (e.g., file busy).\r\n";
const char *const RESP_500 = "500 Syntax error, command unrecognized. This may include errors such as command line too long.\r\n";
const char *const RESP_501 = "501 Syntax error in parameters or arguments.\r\n";
const char *const RESP_502 = "502 Command not implemented.\r\n";
const char *const RESP_503 = "503 Bad sequence of commands.\r\n";
const char *const RESP_504 = "504 Command not implemented for that parameter.\r\n";
const char *const RESP_530 = "530 Not logged in, user or password incorrect.\r\n";
const char *const RESP_532 = "532 Need account for storing files.\r\n";
const char *const RESP_533 = "553 Requested action not taken. File name not allowed.\r\n";
const char *const RESP_550 = "550 Permission deny.\r\n";
const char *const RESP_550EX = "550 Permission deny, %s\r\n";

#endif
