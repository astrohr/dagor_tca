using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ASCOM.DagorApiClient;


namespace ASCOM.Dagor
{

    public class ReprState : IRepr
    {
        public int n { get; set; }
    }

    class DomeApiClient : ApiClient<DefaultReadyRepr>
    {
        public DomeApiClient(string Proto, string Server, int Port) : base(Proto, Server, Port)
        { }

        protected override string _DevPath()
        {
            return "dome";
        }

        // TO-DO ...
       
    }
}
