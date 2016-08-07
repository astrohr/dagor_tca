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
        // TO-DO   
    }

    class TelescopeApiClient : ApiClient<DefaultReadyRepr>
    {
        public TelescopeApiClient(string Proto, string Server, int Port) : base(Proto, Server, Port)
        { }

        protected override string _DevPath()
        {
            return "telescope";
        }

        // TO-DO ...
    }
}
