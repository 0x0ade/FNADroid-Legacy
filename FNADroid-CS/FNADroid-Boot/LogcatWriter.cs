using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace FNADroid {
	
	public class LogcatWriter : TextWriter {
		
		public override Encoding Encoding {
			get {
				return Encoding.UTF8;
			}
		}
		
		private readonly StringBuilder builder = new StringBuilder();
		
		public string Tag = "FNADroid.LogcatWriter";
		public Log.Level Level = Log.Level.INFO;
		
		public override void Write(string value) {
			int nl = value.IndexOf('\n');
			builder.Append(value);
			if (nl < 0) {
				return;
			}
			string complete = builder.ToString();
			builder.Clear();
			int pos = 0;
			while ((nl = complete.IndexOf('\n', pos)) > 0) {
				Log.Line(Level, Tag, complete.Substring(pos, nl));
				pos = nl + 1;
			}
			if (pos + 1 < complete.Length) {
				builder.Append(complete.Substring(pos + 1));
			}
		}
		
		public override void WriteLine(string value) {
			Write(value);
			Write("\n");
		}
		
		public override void Flush() {
			if (builder.Length == 0) {
				return;
			}
			Write("\n");
		}
		
	}
	
}
