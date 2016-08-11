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

        public int getFanState(int fanId)
        {
            try
            {
                int repr_state = ExecuteGET<int>(fanId.ToString());
                return repr_state;
            }
            catch (Exception ex)
            {
                // TODO log exception
                throw new NotConnectedException();
            }
        }

        public void setFanState(int fanId, int fanState)
        {
            try
            {         
                int repr_state = ExecutePUT<int, int>(fanId.ToString(), fanState);                
                if (repr_state != fanState)
                {
                    throw new ASCOM.ValueNotSetException(
                        "setFanState: Unexpected value returned. Expected: " + fanState.ToString() +
                        " Returned: " + repr_state.ToString());                         
                }
            }
            catch (Exception ex)
            {
                // TODO log exception
                throw new NotConnectedException();
            }
        }
    }
}
