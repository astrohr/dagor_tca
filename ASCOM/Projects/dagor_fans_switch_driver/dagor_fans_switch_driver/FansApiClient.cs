using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using ASCOM.DagorApiClient;

namespace ASCOM.DagorFans
{
    public class ReprState : IRepr
    {
        
    }

    class FansApiClient : ApiClient<DefaultReadyRepr>
    {
        public FansApiClient(string Proto, string Server, int Port) : base(Proto, Server, Port)
        { }

        protected override string _DevPath()
        {
            return "fans";
        }

        public bool IsIdle
        {
            get
            {
                return true;
            }
        }       
    }
}
