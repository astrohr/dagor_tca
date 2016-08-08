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

        public double getFanState(int fanId)
        {
            try
            {
                double repr_state = ExecuteGET<double>(fanId.ToString());
                return repr_state;
            }
            catch (Exception ex)
            {
                // TODO log exception
                throw new NotConnectedException();
            }
        }

        public bool setFanState(int fanId, double fanState)
        {
            try
            {
                bool repr_state = ExecutePUT<bool, double>(fanId.ToString(), fanState);
                return repr_state;
            }
            catch (Exception ex)
            {
                // TODO log exception
                throw new NotConnectedException();
            }
        }
    }
}
