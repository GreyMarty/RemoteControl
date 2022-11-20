namespace RemoteControl
{
	namespace IO
	{
		namespace Manipulation
		{
			inline bool CommandStreamParser::CanParse::get() 
			{
				return m_stream->Position < m_stream->Length;
			}
		}
	}
}
