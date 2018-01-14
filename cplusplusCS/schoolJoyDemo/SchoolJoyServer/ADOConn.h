#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF","adoEOF")
class ADOConn
{
public:
	ADOConn(){};
	virtual ~ADOConn(){};
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	void OnInitConnect();
	_RecordsetPtr &GetRecordSet(_bstr_t bstrSQL);
	BOOL ExecuteSQL(_bstr_t bstrSQL);
	void ExitConnect();
};