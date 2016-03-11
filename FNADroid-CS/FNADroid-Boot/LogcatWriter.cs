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

                public Action<string> OnWrite;

                public override void Write(string value) {
                        if (OnWrite != null) {
                                OnWrite(value);
                        }
                        base.Write(value);
                }

        }

}
